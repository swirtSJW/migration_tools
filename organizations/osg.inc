<?php

/**
 * @file
 * Office of the Solicitor General OSG.
 *
 * Defines migration classes for the OSG section of justice.gov.
 */

/**
 * Migrates .html files from /osg to page nodes.
 *
 * @package doj_migration
 * @subpackage osg
 */
class OsgPageMigration extends JusticeHtmlToPageMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {

    $dir_json = '["osg","osg\/aboutosg","osg\/opportunities","osg\/test"]';

    // Define source directories.
    $source_dirs = drupal_json_decode($dir_json);

    $options = array('recurse' => FALSE);

    // Parent constructor will set $this->source, $this->destination, and
    // $this->map.
    parent::__construct($arguments, $source_dirs, NULL, $options);
    $this->dependencies = array('Organization');
    $this->description = t('Migrates pages from Office of Legal Policy section.');
    $this->addFieldMapping('og_group_ref')->defaultValue('osg')->sourceMigration('Organization');
  }

}

/**
 * Migrates files (*.pdf) from /osg.
 *
 * @package doj_migration
 * @subpackage osg
 */
class OsgFileMigration extends JusticeDeployableBinaryFileMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {
    $this->description = t('Migrates non-image files from the olp subdirectory.');
    $this->dependencies = array('Organization');

    // Match .pdf files only.
    $source_dirs = array(
      'osg',
    );
    $regex = '/.*\.(pdf|txt|rtf|doc|docx|xls|xlsx|csv|mp3|mp4|wpd|wp|qpw|xml|ppt|pptx)/';
    $dest_dir = 'public:///osg/docs';

    $options = array('recurse' => FALSE);

    // This will setup $this->map, $this->destination, and $this->source.
    // It will also add field mappings for file location and redirects, which
    // relies on prepareRow() defining $row->filepath and $row->legacy_path.
    parent::__construct($arguments, $source_dirs, $dest_dir, $regex, $options);
  }

  /**
   * {@inheritdoc}
   */
  public function getOrganizationAbbreviation() {
    return "osg";
  }
}

/**
 * Class OsgBriefMigration.
 * @package doj_migration
 */
class OsgBriefMigration extends JusticeHtmlToNodeMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {

    // Define the fields that will be derived from the static files.
    $source_fields = array(
      'title' => t('Title'),
      'field_brief_body' => t('Body'),
      'field_brief_docket' => t('Docket number'),
      'field_brief_sc_term' => t('Supreme Court term'),
      'field_brief_filing_date' => t('Filing date'),
      'field_brief_topic' => t('Topic'),
      'field_brief_court_level' => t('Court level'),
      'field_brief_federal_court' => t('Federal court'),
      'field_brief_links' => t('Links'),
      'field_brief_attachments' => t('Attachments'),
    );

    $source_dirs = array(
      'osg/briefs',
    );

    $regex = '/toc3index\.html/';

    $options = array('recurse' => TRUE);

    // Instantiate the parser object that will parse a single HTML file into
    // chunks that will be translated into nodes.
    $parser = new BriefChunkParser();

    // Calling parent constructor will set $this->source and $this->map.
    parent::__construct($arguments, $source_fields, $source_dirs, $regex, $options, $parser);

    // The destination is the page content type.
    $this->destination = new MigrateDestinationNode('brief');
    $this->addFieldMapping('og_group_ref')->defaultValue('osg')->sourceMigration('Organization');

    // Define non-simple field mappings.
    $this->addFieldMapping('field_brief_body', 'body');
    $this->addFieldMapping('field_brief_body:format')->defaultValue('wysiwyg');
    $this->addFieldMapping('field_brief_docket', 'field_brief_docket');
    $this->addFieldMapping('field_brief_sc_term', 'field_brief_sc_term');
    $this->addFieldMapping('field_brief_filing_date', 'field_brief_filing_date');
    $this->addFieldMapping('field_brief_topic', 'field_brief_topic');
    $this->addFieldMapping('field_brief_court_level', 'field_brief_court_level')->defaultValue('supreme_court');
    $this->addFieldMapping('field_brief_type', 'field_brief_type');
    $this->addFieldMapping('field_brief_links', 'field_brief_links');

    $this->addFieldMapping('field_brief_attachments', 'attachment_filename');
    $this->addFieldMapping('field_brief_attachments:file_class')
      ->defaultValue('MigrateFileUri');
    $this->addFieldMapping('field_brief_attachments:destination_dir', 'destination_dir');
    $this->addFieldMapping('field_brief_attachments:source_dir', 'source_dir');
  }

  /**
   * {@inheritdoc}
   */
  public function prepareRow($row) {
    if (parent::prepareRow($row) === FALSE) {
      return FALSE;
    }

    // Parse the filename to extract tags and year data.
    // $row->fileid will look something like:
    // /osg/briefs/2014/3mer/2mer/toc3index.html-?MIGRATECHUNK?-No. 13-534.
    $fileid_parts = explode('-?MIGRATECHUNK?', $row->fileid);
    $file_path = $fileid_parts[0];
    $chunk_id = $fileid_parts[1];

    // Extract date from url.
    preg_match('/[0-9]{4}/', $file_path, $matches);
    if ($matches) {
      $year = $matches[0];
      $row->field_brief_sc_term = $matches[0];
      unset($matches);
    }

    // Generate file destination path based on article date.
    $row->destination_dir = "public:///osg/briefs/$year/01/01";

    // Extract docket number.
    preg_match('/[A-Z0-9-]/i', $chunk_id, $matches);
    if ($matches) {
      $row->field_brief_docket = $matches[0];
    }

    // Parse attachment URL.
    $link = $this->sourceParser->queryPath->find('a');
    if ($link) {
      $attachment_url = $link->attr('href');
      $attachment_ext = pathinfo($attachment_url, PATHINFO_EXTENSION);

      $row->attachment_filename = basename($attachment_url);
      $row->source_dir = $this->baseDir . '/' . ltrim(dirname($file_path), '/');

      $attachment_filepath = $row->source_dir . '/' . $row->attachment_filename;
      $link->remove();

      // Parse contents of attached pdf file.
      if ($attachment_ext == 'pdf') {
        try {
          // Set brief topic by searching for substring in attachment url.
          $brief_topics = array(
            'zadm' => 'Admiralty & Maritime',
            'zadlaw' => 'Administrative Law',
            'zatr' => 'Antitrust (including FTC antitrust)',
            'zbkg' => 'Banking (FDIC, Fed., OCC, RTC)',
            'zbkr' => 'Bankruptcy',
            'zciv1' => 'Civil Div. I: General (e.g., DOT, FEC, FOIA, FTC (except antitrust), HUD)',
            'zciv2' => 'Civil Div. II: HHS (Medicare, Social Security)',
            'zcrt' => 'Civil Rights (including EEOC)',
            'zcom' => 'Communications (FCC)',
            'zcon' => 'Constitutional',
            'zcrim' => 'Criminal (including Habeas/2255)',
            'zcus' => 'Customs & International Trade',
            'zedu' => 'Education',
            'zerisa' => 'ERISA',
            'zforf' => 'Forfeiture (civil & criminal)',
            'zhls' => 'National Security & Homeland Security',
            'zimm' => 'Immigration, Naturalization, & Citizenship',
            'zind' => 'Indian Law',
            'zlab' => 'Labor & Employment: FLRA, Labor Dep’t (not ERISA), LHWCA, MSPB, NLRB, OSHA',
            'zenv' => 'Natural Resources: ENRD, EPA, FERC, Interior, water rights',
            'zorig' => 'Original Jurisdiction',
            'zpto' => 'Patent, Trademark, & Copyright',
            'zsec' => 'Securities Regulation (SEC)',
            'ztax' => 'Tax',
            'ztort' => 'Torts (FTCA, Bivens Actions, § 1983, Qualified Immunity)',
            'ztfa' => 'Treaties and Foreign Affairs',
            'zmisc' => 'Other',
          );

          $pdf_metadata = $this->getPdfMetadata($attachment_filepath);
          if (!empty($pdf_metadata['Keywords'])) {
            $topics = $this->findNeedleSubstrings($brief_topics, $pdf_metadata['Keywords']);
            foreach ($topics as $topic) {
              doj_migration_create_term($topic, 'topic');
            }
            $row->field_brief_topic = implode('|', $topics);
          }
        }
        catch (Exception $e) {
          $message = t('Could not parse attached PDF for @fileid. Exception thrown: @exception', array('@fileid' => $row->fileid, '@exception' => $e->getMessage()));
          $this->queueMessage($message);
          watchdog('doj_migration', $message);
        }
      }

      // Set brief type by searching for substring in attachment url.
      $brief_types = array(
        '.pet.aa' => 'Petition for Writ of Certiorari',
        '.pet.app' => 'Appendix to Petition',
        '.pet.rep' => 'Petition Stage Reply Brief',
        '.pet.ami.inv' => 'Petition Stage Amicus Brief (Invitation)',
        '.pet.ami' => 'Petition Stage Amicus Brief (Uninvited)',
        '.pet.sup' => 'Petition Stage Supplemental Brief',
        '.resp' => 'Petition Stage Response',
        '.mer.aa' => 'Merits Stage Brief',
        '.mer.rep' => 'Merits Stage Reply Brief',
        '.mer.ami' => 'Merits Stage Amicus Brief',
        '.ami.mer' => 'Merits Stage Amicus Brief',
        '.mer.post' => 'Post Argument Brief',
        '.Original.resp' => 'Original Actions: All',
      );

      $types = $this->findNeedleSubstrings($brief_types, $attachment_url, FALSE);
      $row->field_brief_type = implode('|', array_values($types));
    }

    $docket_number = doj_migration_find_docket_number($row->filedata);
    $row->field_brief_docket = $docket_number;
    $row->title = StringCleanUp::stripFunkyChars(ltrim(str_replace($docket_number, '', $this->sourceParser->queryPath->text()), ' :'));
    $row->title = mb_strimwidth(trim($row->title, ' \t\n\r\0\x0B,|'), 0, 255, "...");

    // If the title is empty attempt to use PDF metadata for title.
    if (empty($row->title) && !empty($pdf_metadata['Title'])) {
      $row->title = $pdf_metadata['Title'];
    }

    $row->body = '';
    unset($row->legacy_path);
  }

  /**
   * Searching a string haystack for occurrences of substring needles.
   *
   * This function is case insensitive.
   *
   * @param array $needles
   *   An array of substring needles to search for in the haystack.
   *
   * @param string $haystack
   *   A string.
   *
   * @param bool $multiple
   *   If FALSE, a maximum of one value will be returned in the array.
   *
   * @return array
   *   An array of all found needles.
   */
  public function findNeedleSubstrings($needles, $haystack, $multiple = TRUE) {
    $values = array();
    foreach ($needles as $needle_key => $needle_title) {
      if (stristr($haystack, $needle_key)) {
        $values[$needle_key] = $needle_title;
        if (!$multiple) {
          return $values;
        }
      }
    }

    return $values;
  }


  /**
   * Extracts metadata from pdf file.
   *
   * @param string $pdf_file_path
   *   The absolute file path of the pdf on the local system.
   *
   * @return array
   *   An associative array of pdf metadata.
   */
  public function getPdfMetadata($pdf_file_path) {
    $pdfinfo_bin = variable_get('pdfinfo_binary', '/home/doj/xpdf/bin64/pdfinfo');
    $command = "$pdfinfo_bin $pdf_file_path";
    $output = shell_exec($command);
    $pdf_info_rows = explode("\n", $output);
    $pdf_metadata = array();
    foreach ($pdf_info_rows as $row) {
      $columns = explode(':', $row);
      $pdf_metadata[$columns[0]] = $columns[1];
    }

    return $pdf_metadata;
  }

  /**
   * Extracts the contents of a pdf file.
   *
   * @param string $pdf_file_path
   *   The absolute file path of the pdf on the local system.
   *
   * @return string
   *   The text content of the pdf.
   *
   * @throws \XPDF\Exception\BinaryNotFoundException
   */
  public function getPdfContents($pdf_file_path) {
    // Get PDF contents.
    $pdftotext_bin = variable_get('pdftotext_binary', '/home/doj/xpdf/bin64/pdftotext');
    $pdf_parser = XPDF\PdfToText::create(array('pdftotext.binaries' => $pdftotext_bin));
    $pdf_contents = $pdf_parser->getText($pdf_file_path);

    return $pdf_contents;
  }
}

/**
 * Parses a single brief index page into multiple briefs.
 *
 * @package doj_migration
 * @subpackage core
 */
class BriefChunkParser extends JusticeChunkParser {

  /**
   * Sets $this->chunks.
   */
  public function setChunks() {
    // There are multiple patterns used on brief pages.
    // @see http://www.justice.gov/osg/briefs/1985/index.html
    // @see http://www.justice.gov/osg/briefs/2014/0responses/toc3index.html
    $this->chunks = $this->sourceParser->queryPath->find('.brieflist > div');
    if (!$this->chunks) {
      $this->chunks = $this->sourceParser->queryPath->find('.bodytext > p');
    }
    if (!$this->chunks) {
      $this->chunks = array();
    }
  }

  /**
   * {@inheritdoc}
   */
  public function getChunkIDs() {
    $chunks = $this->getChunks();

    $ids = array();

    foreach ($chunks as $chunk) {
      // The ID of each gallery image will be the href of the image file.
      $text = $chunk->text();
      $id = doj_migration_find_docket_number($text);

      // Filter out empty strings.
      if ($id) {
        $ids[] = $id;
      }
    }

    return $ids;
  }

  /**
   * {@inheritdoc}
   */
  public function getChunk($chunk_id) {
    $chunks = $this->getChunks();
    $chunk = $chunks->filterPreg('|' . $chunk_id . '|');

    if ($chunk->is('div')) {
      $data = $chunk->innerHtml();
    }
    else {
      $data = $chunk->parent('p')->innerHtml();
    }

    if (!$data) {
      $data = $chunk->parent('div')->innerHtml();
    }
    if (!$data) {
      return 'Data could not be migrated';
    }
    else {
      return $data;
    }
  }
}

/**
 * Extracts the docket number from a string of text using regex.
 *
 * @param string $string
 *   A string containing the docket number
 *
 * @return string
 *   The extracted docket number.
 */
function doj_migration_find_docket_number($string) {
  // In this regex we are ignoring matches USDOJ:, OSG:, etc. with a negative
  // look behind. This should match:
  // @codingStandardsIgnoreStart
  // 96-1590B:
  // No. 06-1285
  // No. 135,
  // No. 97-1942
  // Nos. 99-464,
  // No. 132, Original:
  // @codingStandardsIgnoreEnd
  preg_match('#(([A-Z0-9-]+)(?<!USDOJ)(?<!OSG):)|(No(s?)\. ([A-Z0-9-]+))(([ ,]|(and))*(?!Original)([A-Z0-9-]+))*#', $string, $matches);
  if ($matches) {
    $docket_number = $matches[0];
    return $docket_number;
  }
  return FALSE;
}

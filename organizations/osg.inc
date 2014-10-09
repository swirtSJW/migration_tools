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

    $regex = '/^(toc3)?index\.html/';

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
    $this->addFieldMapping('field_brief_attachments:file_replace')->defaultValue(FILE_EXISTS_REUSE);
  }

  /**
   * {@inheritdoc}
   */
  public function prepareRow($row) {

    if (parent::prepareRow($row) === FALSE) {
      return FALSE;
    }

    // Unset variables that were set by parent methods.
    $row->body = '';
    unset($row->legacy_path);

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

    // The attachmentRedirect map property is an array of legacy URLs for every
    // file that is added as an attachment to this row. This property will
    // later be used in complete() to create redirects to the new file
    // location.
    $row->attachmentRedirects = array();

    // Parse attachment URL. Examples of attachment patterns:
    // http://www.justice.gov/osg/briefs/1998/3mer/1ami/toc3index.html
    // http://www.justice.gov/osg/briefs/1983/index.html
    $links = $this->sourceParser->queryPath->find('a');
    foreach ($links as $link) {
      $link_url = $link->attr('href');
      $link->remove();

      // Get more information about the linked file.
      $link_ext = pathinfo($link_url, PATHINFO_EXTENSION);
      $link_filename = basename($link_url);
      $link_dir_path = ltrim(dirname($file_path), ' /');
      $link_url_abs = url($link_dir_path . '/' . $link_filename, array('absolute' => TRUE, 'base_url' => 'http://www.justice.gov'));

      // Determine the file path to the linked file on the local system.
      $source_dir = $this->baseDir . '/' . $link_dir_path;
      $system_file_path = $source_dir . '/' . $link_filename;

      switch ($link_ext) {
        case 'pdf':

          $pdf_metadata = $this->getPdfMetadata($system_file_path);
          if (!empty($pdf_metadata['Keywords'])) {
            $row->field_brief_topic = $this->getBriefTopic($pdf_metadata['Keywords']);
          }

          // Intentional fall through.
        case 'txt':
          $row->attachmentRedirects[] = $link_url_abs;
          $row->source_dir = $source_dir;
          $row->attachment_filename = $link_filename;
          $row->field_brief_type = $this->getBriefType($link_url);

          break;

        case 'html':
          $attached_html = new SourceParser($system_file_path, file_get_contents($system_file_path));

          // Remove redundant PDF links from body.
          $row->body = $attached_html->getBody();
          $row->body = preg_replace('|<a[^>]*>View PDF Version<\/a>|', " ", $row->body);

          // Remove empty p tags. We are not removing p tags containing &nbsp;
          $row->body = preg_replace("/\r|\n/", " ", $row->body);

          // Remove <tt> tags.
          $row->body = preg_replace('/<tt>|<\/tt>/', " ", $row->body);

          break;
      }
    }

    $docket_number = doj_migration_find_docket_number($row->filedata);
    $row->field_brief_docket = $docket_number;

    // Clean up title.
    $row->title = $this->sourceParser->queryPath->text();
    $row->title = str_replace($docket_number, '', $row->title);
    $row->title = StringCleanUp::stripFunkyChars($row->title);
    $row->title = trim($row->title, " \t\n\r\0\x0B,|-:");
    $row->title = mb_strimwidth($row->title, 0, 255, "...");
    $row->title = StringCleanUp::superTrim($row->title);
    // Replace double spaces with single spaces
    $row->title = preg_replace('/(\s){2,}/', ' ', $row->title);
    $row->title = preg_replace('/[\pZ\pC]{2,}|[\pZ\pC]{2,}/u', ' ', $row->title);

    // If the title is empty attempt to use PDF metadata for title.
    if (empty($row->title)) {
      if (!empty($pdf_metadata['Title'])) {
        $row->title = $pdf_metadata['Title'];
      }
      // If we still don't have a title at this point, skip the row.
      else {
        return FALSE;
      }
    }
  }

  /**
   * {@inheritdoc}
   */
  public function complete($entity, $row) {
    parent::complete($entity, $row);

    if (module_exists('redirect')) {
      $this->createAttachmentRedirect($entity, $row->attachmentRedirects, 'field_brief_attachments');
    }
  }

  /**
   * {@inheritdoc}
   */
  public function prepareRollback($entity_id) {
    $entities = entity_load('node', $entity_id);
    $entity = reset($entities);

    $this->rollbackAttachmentRedirect($entity, 'field_brief_attachments');
  }

  /**
   * Gets list of brief types by searching a URL for valid substring match.
   *
   * @param $attachment_url
   *   The url of the pdf. This string will be searched for key strings.
   *
   * @return string
   */
  public function getBriefType($attachment_url) {
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

    return implode('|', array_values($types));
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
   * Extracts as valid brief topic from a string.
   *
   * @param string $string
   *   A string that might contain a brief topic.
   *
   * @return string|NULL
   *   A list of brief topics, separated by '|'.
   *
   */
  public function getBriefTopic($string) {

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

    $topics = $this->findNeedleSubstrings($brief_topics, $string);
    foreach ($topics as $topic) {
      doj_migration_create_term($topic, 'topic');
    }

    return implode('|', $topics);
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
    // @see http://www.justice.gov/osg/briefs/2006/2pet/7pet/toc3index.html
    // @see http://www.justice.gov/osg/briefs/2014/0responses/toc3index.html
    $this->chunks = $this->sourceParser->queryPath->find('.brieflist > div');
    if (!$this->chunks || !$this->chunks->text()) {
      $this->chunks = $this->sourceParser->queryPath->find('.bodytext > p');
    }
    if (!$this->chunks || !$this->chunks->text()) {
      $this->chunks = array();
      watchdog('doj_migration', 'Could not find chunks for JusticeChunkParser child class @class_name', array('@class_name' => get_class()), WATCHDOG_ERROR);
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
      $data = $chunk->innerHtml();
    }

    if (!$data) {
      $data = $chunk->parent('div')->innerHtml();
    }
    if (!$data) {
      watchdog('doj_migration', 'Data could not be migrated for chunk @chunk_id', array('@chunk_id' => $chunk_id), WATCHDOG_ERROR);
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
  // No. 83-1386, No. 83-1721 and No. 83-1838
  // No. 83-1569 & No. 83-1733
  // @codingStandardsIgnoreEnd
  preg_match('#(([A-Z0-9-]+)(?<!USDOJ)(?<!OSG):)|(No(s?)\. ([A-Z0-9-]+))(([ ,]|(and)|[ ]|No\.)*(?!Original)([A-Z0-9-]+))*#', $string, $matches);
  if ($matches) {
    $docket_number = $matches[0];
    return $docket_number;
  }
  return FALSE;
}

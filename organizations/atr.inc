<?php

/**
 * @file
 * Defines migration classes for Antitrust Division.
 */

/**
 * Node ID of ATR organization.
 */
define('ATR_NID', 1630);

/**
 * Migrates .html files from /atr to page nodes.
 *
 * @package doj_migration
 * @subpackage atr
 */
class AtrPageMigration extends NGJusticeHtmlToPageMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {

    // Define source directories.
    $source_dirs = array(
      0 => 'atr',
      725 => 'atr/contact/jobs',
      726 => 'atr/contact/jobs/paq',
      727 => 'atr/contact',
      728 => 'atr/foia',
      729 => 'atr/foia/divisionmanual',
      730 => 'atr/foia/frito-lay',
      731 => 'atr/foia/rll',
      732 => 'atr/hmerger',
      733 => 'atr/icpac',
      734 => 'atr/public',
      735 => 'atr/public/216254',
      736 => 'atr/public/220241',
      737 => 'atr/public/231424',
      738 => 'atr/public/articles',
      739 => 'atr/public/busreview',
      740 => 'atr/public/comments',
      741 => 'atr/public/comments/sec271',
      742 => 'atr/public/comments/sec271/ameritech',
      743 => 'atr/public/comments/sec271/bellatlantic',
      744 => 'atr/public/comments/sec271/bellsouth',
      745 => 'atr/public/comments/sec271/qwest',
      746 => 'atr/public/comments/sec271/sbc',
      747 => 'atr/public/comments/sec271/verizon',
      748 => 'atr/public/criminal',
      749 => 'atr/public/divisionmanual/supporting_documents',
      750 => 'atr/public/eag',
      751 => 'atr/public/eag/221240',
      752 => 'atr/public/eag/221241',
      753 => 'atr/public/eag/221243',
      754 => 'atr/public/eag/221246',
      755 => 'atr/public/eag/221876',
      756 => 'atr/public/eag/221877',
      757 => 'atr/public/eag/221883',
      758 => 'atr/public/eag/228687',
      759 => 'atr/public/eag/228709',
      760 => 'atr/public/eag/232668',
      761 => 'atr/public/eag/232692',
      762 => 'atr/public/electronic_discovery',
      763 => 'atr/public/guidelines',
      764 => 'atr/public/health_care',
      765 => 'atr/public/health_care/204694',
      766 => 'atr/public/hearings/ip',
      767 => 'atr/public/hearings/single_firm',
      768 => 'atr/public/hearings/single_firm/bio',
      769 => 'atr/public/hearings/single_firm/comments',
      770 => 'atr/public/hearings/single_firm/docs',
      771 => 'atr/public/hearings/single_firm/docs/218780',
      772 => 'atr/public/hearings/single_firm/docs/219980',
      773 => 'atr/public/hearings/single_firm/docs/222104',
      774 => 'atr/public/international',
      775 => 'atr/public/international/docs',
      800 => 'atr/public/real_estate',
      801 => 'atr/public/reports',
      802 => 'atr/public/speeches',
      803 => 'atr/public/speeches/212266',
      804 => 'atr/public/speeches/221173a',
      805 => 'atr/public/speeches/227291',
      806 => 'atr/public/speeches/232716',
      807 => 'atr/public/taskforces',
      808 => 'atr/public/testimony',
      809 => 'atr/public/workshops',
      810 => 'atr/public/workshops/ag2010',
      811 => 'atr/public/workshops/airlines2008',
      812 => 'atr/public/workshops/docs',
      813 => 'atr/public/workshops/docs/202661',
      814 => 'atr/public/workshops/ip',
      815 => 'atr/public/workshops/rewbios',
      816 => 'atr/public/workshops/rewcom',
      817 => 'atr/public/workshops/rewcom/213351',
      818 => 'atr/public/workshops/techassist2008',
      819 => 'atr/public/workshops/techassist2008/bios',
      820 => 'atr/public/workshops/telecom2007',
      821 => 'atr/public/workshops/telecom2007/bios',
      822 => 'atr/public/workshops/telecom2007/submissions/227645',
      823 => 'atr/public/workshops/telecom2007/submissions',
      824 => 'atr/public/workshops/telecom2007/submissions/227819',
      825 => 'atr/public/workshops/telecom2007/submissions/227839',
      826 => 'atr/public/workshops/telecom2007/submissions/227842',
      827 => 'atr/public/workshops/telecom2007/submissions/228008',
      828 => 'atr/public/workshops/telecom2007/submissions/228010',
      829 => 'atr/public/workshops/telecom2007/submissions/228096',
      830 => 'atr/public/workshops/telecom2007/submissions/228097',
      831 => 'atr/public/workshops/telecom2007/submissions/228098',
      832 => 'atr/public/workshops/telecom2007/submissions/228099',
      833 => 'atr/public/workshops/telecom2007/submissions/228100',
      834 => 'atr/public/workshops/telecom2007/submissions/228101',
      835 => 'atr/public/workshops/telecom2007/submissions/228972',
      836 => 'atr/public/workshops/telecom2007/submissions/229055',
      837 => 'atr/victim',
      838 => 'atr/about',
      847 => 'atr/espanol',
      848 => 'atr/events/hearings/comphearing',
      849 => 'atr/events/hearings/hchearing',
      850 => 'atr/events',
      851 => 'atr/events/workshops/meworkshop',
      852 => 'atr/public/appellate',
      853 => 'atr/public/closing',
      854 => 'atr/public/division-update/2011',
      855 => 'atr/public/division-update/2011/gallery',
      856 => 'atr/public/division-update/2012',
      857 => 'atr/public/division-update/2012/gallery',
      858 => 'atr/public/division-update/2013',
      859 => 'atr/public/division-update/2014',
      860 => 'atr/public/divisionmanual',
      861 => 'atr/public/guidelines/horiz_book',
      862 => 'atr/public/international/gallery',
      863 => 'atr/public/taskforces/eemtaskforce',
      864 => 'atr/public/update/2010',
      865 => 'atr/public/update/2010/gallery',
      866 => 'atr/public/update',
      867 => 'atr/public/workshops/cpp',
      868 => 'atr/public/workshops/healthcare/2015/02',
      869 => 'atr/public/workshops/mfn',
      870 => 'atr/public/workshops/pae',
      871 => 'atr/RePEc/doj/compad',
      872 => 'atr/RePEc/doj/eagpap',
      873 => 'atr/RePEc/doj',
      874 => 'atr/rss',
    );

    $options = array('recurse' => FALSE);

    $arguments['source_parser_class'] = "AtrPageSourceParser";

    // Configuring Obtainers.
    $title = new ObtainerInfo('title', "AtrObtainTitlePage");
    $title->addMethod('findClassBreadcrumbMenuContentLast');
    $title->addMethod('pluckSelector', array("h1", 1));
    $title->addMethod('pluckSelector', array("title", 1));

    $arguments['obtainers_info'][] = $title;

    // Parent constructor will set $this->source, $this->destination, and
    // $this->map.
    parent::__construct($arguments, $source_dirs, NULL, $options);
    $this->dependencies = array('Organization');
    $this->description = t('Migrates pages from the Antitrust Division.');
    $this->addFieldMapping('og_group_ref')
    ->defaultValue('atr')
    ->sourceMigration('Organization');
  }

  /**
   * {@inheritdoc}
   */
  public function prepareRow($row) {
    $skip_these = array();
    if (doj_migration_skip_file($row->fileid, $skip_these) || parent::prepareRow($row) === FALSE) {
      return FALSE;
    }
  }
}

/**
 * Migrates files (*.pdf) from /atr.
 *
 * @package doj_migration
 * @subpackage atr
 */
class AtrFileMigration extends JusticeDeployableBinaryFileMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {
    $this->description = t('Migrates non-image files from the atr subdirectory.');
    $this->dependencies = array('Organization');

    // Match .pdf files only.
    $source_dirs = array(
      'atr',

    );

    $regex = '/.*\.(pdf|txt|rtf|doc|docx|xls|xlsx|csv|mp3|mp4|wpd|wp|qpw|xml|ppt|pptx|db|rdf|swf|tex|vcf|wav|wps|xht|xml|zip)/i';
    $dest_dir = 'public:///atr/docs';

    $options = array('recurse' => TRUE);

    // This will setup $this->map, $this->destination, and $this->source.
    // It will also add field mappings for file location and redirects, which
    // relies on prepareRow() defining $row->filepath and $row->legacy_path.
    parent::__construct($arguments, $source_dirs, $dest_dir, $regex, $options);
  }


  /**
   * {@inheritdoc}
   */
  public function getOrganizationAbbreviation() {
    return "atr";
  }
}

/**
 * Migrates press releases from /atr.
 *
 * @package doj_migration
 * @subpackage atr
 */
class AtrPressReleaseMigration extends NGJusticeHtmlToPressReleaseMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {
    $arguments['component'] = "Antitrust Division";
    $arguments['source_parser_class'] = "AtrPressSourceParser";
    $arguments['component_tid'] = 376;
    // Configuring Obtainers.
    $title = new ObtainerInfo('title', "AtrObtainTitlePress");
    $title->addMethod('pluckAnySelectorUntilValid', array('h1'));
    $title->addMethod('pluckSelector', array("#contentstart > div > h2", 2));
    $title->addMethod('pluckSelector', array("h2", 1));
    $title->addMethod('pluckSelector', array(".contentSub > div > p[align='center'] > strong", 1));
    $title->addMethod('pluckSelector', array(".contentSub > div > div > p > strong", 1));
    $title->addMethod('pluckSelector', array("#headline", 1));
    $title->addMethod('pluckSelector', array("p > strong > em", 1));
    $title->addMethod('pluckSelector', array("p > strong > u", 1));
    $title->addMethod('pluckSelector', array("#contentstart > div > h2", 1));

    $date = new ObtainerInfo('date', "AtrObtainDate");
    $date->addMethod('findDateInTableAfterBr');
    $date->addMethod('pluckSelector', array("p[align='center']", 1));
    $date->addMethod('pluckSelector', array("#contentstart > p", 1));
    $date->addMethod('pluckSelector', array(".newsRight", 1));
    $date->addMethod('pluckSelector', array(".BottomLeftContent", 1));
    $date->addMethod('pluckProbableDate');

    $pr_number = new ObtainerInfo('prNumber', "AtrObtainPrNumber");
    $pr_number->addMethod('findLastP');

    $arguments['obtainers_info'][] = $title;
    $arguments['obtainers_info'][] = $date;
    $arguments['obtainers_info'][] = $pr_number;

    $source_dirs = array(
      797 => 'atr/public/press_releases/2013',
      798 => 'atr/public/press_releases/2014',
      799 => 'atr/public/press_releases/2015',
    );

    parent::__construct($arguments, $source_dirs);

    $this->addFieldMapping('og_group_ref')
      ->defaultValue('atr')
      ->sourceMigration('Organization');

    // Add file attachment.
    $this->addFieldMapping('field_pr_attachment', 'filename');
    $this->addFieldMapping('field_pr_attachment:file_class')->defaultValue('MigrateFileUri');
    $this->addFieldMapping('field_pr_attachment:source_dir', 'source_dir');
    $this->addFieldMapping('field_pr_attachment:destination_dir', 'destination_dir');
    $this->addFieldMapping('field_pr_attachment:file_replace')->defaultValue(FILE_EXISTS_REUSE);

    $this->addFieldMapping('context')->defaultValue('no_right_sidebar');
  }

  /**
   * {@inheritdoc}
   */
  public function prepareRow($row) {
    parent::prepareRow($row);

    // Generate filepath based on article date.
    $time_created = strtotime($row->field_pr_date);
    $year_created = date('Y', $time_created);
    $month_created = date('m', $time_created);
    $date_created = date('d', $time_created);

    // Set up our attachment-related arrays.
    $row->attachmentRedirects = array();
    $row->filename = array();
    $row->source_dir = array();
    $row->destination_dir = array();

    // An array of pdfLinks is created when the source parse is initialized,
    // in AtrPressReleaseSourceParser::cleanHtml().
    foreach ($this->sourceParser->fileLinks as $file_link) {
      // Add redirects for the attachment's legacy URL.
      $row->attachmentRedirects[] = $file_link;
      $row->filename[] = basename($file_link);
      $row->source_dir[] = variable_get("doj_migration_base_dir") . '/' . dirname($file_link) . '/';
      $row->destination_dir[] = "public:///atr/press_release/$year_created/$month_created/$date_created";
    }
  }
}


/**
 * Migrates .html files from /civil to page nodes.
 *
 * @package doj_migration
 * @subpackage civil
 */
class AtrCaseMigration extends JusticeHtmlToCaseMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {
    // Define source directories.
    $source_dirs = array(
      'atr/cases',
    );

    $options = array('recurse' => TRUE);
    $arguments['source_parser_class'] = "AtrCaseSourceParser";

    // Parent constructor will set $this->source, $this->destination, and
    // $this->map.
    parent::__construct($arguments, $source_dirs, NULL, $options);
    $this->dependencies = array('Organization');
    $this->description = t('Migrates cases from the Antitrust Division.');
    $this->addFieldMapping('og_group_ref')->defaultValue('atr')->sourceMigration('Organization');
  }
}

/**
 * SourceParser for /atr pages.
 *
 * @package doj_migration
 * @subpackage atr
 */
class AtrPageSourceParser  extends NGNodeSourceParser {
  /**
   * {@inheritdoc}
   */
  protected function cleanHtml() {
    parent::cleanHtml();
  }
}

/**
 * SourceParser for /atr press releases.
 *
 * @package doj_migration
 * @subpackage atr
 */
class AtrPressSourceParser extends NGPressReleaseSourceParser {

  /**
   * @var array
   *   A flat array of file links from markup.
   */
  public $fileLinks;

  /**
   * {@inheritdoc}
   */
  protected function cleanHtml() {
    parent::cleanHtml();

    $this->fileLinks = $this->findFileLinks();
    $this->removeDocumentNotice();
    HtmlCleanUp::removeTableBackgrounds($this->queryPath);
  }

  /**
   * Removes the press release notice from the header.
   */
  public function removeDocumentNotice() {
    // Remove header block.
    $table = $this->queryPath->find('table')->first();
    if ($table->attr('bgcolor') == '#c0c0c0') {
      $table->remove();
    }
  }

  /**
   * Finds all anchor links point to a pdf, wpd, or docx extension.
   *
   * @return array
   *   An array of file link URLs.
   */
  public function findFileLinks() {
    $pdf_links = array();
    $links = $this->queryPath->find('a');
    foreach ($links as $link) {
      if (preg_match('/\.(pdf|wpd|docx)/', $link->attr('href'))) {
        $pdf_links[] = $link->attr('href');
      }
    }

    return $pdf_links;
  }
}

/**
 * SourceParser for /atr cases.
 *
 * @package doj_migration
 * @subpackage atr
 */
class AtrCaseSourceParser  extends NGNodeSourceParser {
  /**
   * {@inheritdoc}
   */
  protected function cleanHtml() {
    parent::cleanHtml();
  }
}


/**
 * Obtainer for title property for /atr pages.
 *
 * @package doj_migration
 * @subpackage atr
 */
class AtrObtainTitlePage extends ObtainTitle {
}

/**
 * Obtainer for title property for /atr press releases.
 *
 * @package doj_migration
 * @subpackage atr
 */
class AtrObtainTitlePress extends ObtainTitlePressRelease {
}

/**
 * Obtainer for id property for /atr press releases.
 *
 * @package doj_migration
 * @subpackage atr
 */
class AtrObtainPrNumber extends ObtainId {

  /**
   * Finds last <p> tag in markup.
   *
   * @return string
   *   Found text.
   */
  protected function findLastP() {
    $element = $this->queryPath->find('p')->last();
    $this->setElementToRemove($element);
    $text = $element->text();

    if (preg_match('/([0-9]+)-([0-9]+)/', $text)) {
      return $text;
    }
  }
}

/**
 * Custom obtainer for date property for /atr.
 *
 * @package doj_migration
 * @subpackage atr
 */
class AtrObtainDate extends ObtainDate {

  /**
   * Finds date in table, after <br>.
   *
   * @return string
   *   The date!
   */
  protected function findDateInTableAfterBr() {

    $tables = $this->queryPath->find("table");
    $table_num = 2;
    foreach ($tables as $key => $table) {
      if ($key == $table_num) {
        $td = $this->pluckTableCell($table, 1, 1);
        $td->find('a')->remove();
        $text = $td->find('font')->html();
        $text = $this->trimAtBr($text, 'after');

        return $text;
      }
    }
  }
}

/**
 * Class AtrGalleryMigration.
 */
class AtrGalleryMigration extends JusticeHtmlToGalleryMigration {

  /**
   * Constructor.
   */
  public function __construct($arguments) {
    // Define source directories.
    $source_dirs = array(
      'atr/public/division-update/2011/gallery',
      'atr/public/division-update/2012/gallery',
      'atr/public/update/2010/gallery',
      'atr/public/international/gallery',
    );
    $options = array(
      'recurse' => TRUE,
    );
    $arguments['obtainer_methods'] = array(
      'title' => array(
        'pluckSelector' => array('title'),
      ),
    );

    parent::__construct($arguments, $source_dirs, $options);

    $this->addFieldMapping('og_group_ref')->defaultValue('atr')->sourceMigration('Organization');
  }

  /**
   * {@inheritdoc}
   */
  public function prepareRow($row) {
    if (parent::prepareRow($row) === FALSE) {
      return FALSE;
    }
    $title = $row->title;
    $title = str_replace("U.S. Department of Justice, Antitrust Division, ", "", $title);
    $title = str_replace(", Photo Gallery", "", $title);
    $row->title = $title;
    $row->body = "";
  }
}

/**
 * Class AtrGalleryImagesMigration.
 */
class AtrGalleryImagesMigration extends JusticeHtmlToGalleryImagesMigration {

  /**
   * Constructor.
   */
  public function __construct($arguments) {
    // Define source directories.
    $source_dirs = array(
      'atr/public/division-update/2011/gallery',
      'atr/public/division-update/2012/gallery',
      'atr/public/update/2010/gallery',
      'atr/public/international/gallery',
    );
    $options = array(
      'recurse' => TRUE,
    );
    $arguments['gallery_migration'] = "AtrGallery";
    parent::__construct($arguments, $source_dirs, $options);
  }

  /**
   * {@inheritdoc}
   */
  protected function getGallerySourceDir($row_id) {
    $pieces = explode("index.html", $row_id);
    $source = $this->baseDir . $pieces[0] . "images";
    return $source;
  }
}

/**
 * Migrates speeeches from /atr.
 *
 * @package doj_migration
 * @subpackage usao-sdny
 */
class AtrSpeechMigration extends NGJusticeHtmlToSpeechMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {
    $arguments['component_tid'] = 376;
    $arguments['source_parser_class'] = "AtrSpeechListSourceParser";

    // Define source directories.
    $source_dirs = array(
      'atr/public/speeches',
    );

    $regex = '/^speech-\d{4}\.html/i';
    $options = array('recurse' => FALSE);
    $parser = new AtrSpeechChunkParser();

    // Calling parent constructor will set $this->source and $this->map.
    parent::__construct($arguments, $source_dirs, $options, $regex, $parser);
    $this->dependencies = array('Organization');
    $this->description = t('Migrates speeches from the Antitrust Division.');
    $this->addFieldMapping('og_group_ref')
      ->defaultValue('atr')
      ->sourceMigration('Organization');
  }

  /**
   * {@inheritdoc}
   */
  public function prepareRow($row) {
    $row->chunk = $row->filedata;

    // Parse the filename to extract tags and year data.
    // $row->fileid will look something like:
    // Filename-?MIGRATECHUNK?-Chunk-ID
    $fileid_parts = explode('-?MIGRATECHUNK?-', $row->fileid);
    $row->parent_file_path = $fileid_parts[0];
    $row->chunk_id = $fileid_parts[1];

    if (empty($row->chunk_id)) {
      $this->migrationMessage('No chunk id was present for chunk from file "@file"', array('@file' => $row->fileid), WATCHDOG_ERROR);
      return FALSE;
    }

    $row->title = ObtainTitle::cleanString($row->chunk->text());
    $row->speaker_name = $row->chunk->parent()->prev('h3')->text();
    $row->speaker_role = $row->chunk->parent()->prev('p > em')->text();
    $row->date = $row->chunk->parent()->prev('.news-date')->text();

    // Generate file destination path.
    $row->destination_dir = "public:///atr/speeches";
    $row->speaker = $this->createStaffProfile($row->speaker_name, $row->speaker_role, ATR_NID);

    // Process attached files.
    $similar_files = $this->getAllSimilarlyNamedFiles($row->fileid, '/atr/public/speeches');

    // Attach non-html files.
    foreach ($similar_files as $extension => $file) {
      if (!in_array($extension, array('html', 'htm'))) {
        $row->attachment_filename[] = $file['filename'];
        $row->source_dir[] = dirname($file['uri']);
      }
    }

    // The attachmentRedirect map property is an array of legacy URLs for every
    // file that is added as an attachment to this row. This property will
    // later be used in complete() to create redirects to the new file
    // locations.
    $row->attachmentRedirects = $this->getAttachedFileLegacyUrls($similar_files);

    // Identify which file should be used as canonical for this node.
    // Order of preference is htm, html, pdf.
    $canonical_source_type = (!empty($similar_files['htm'])) ? 'htm' : NULL;
    $canonical_source_type = (empty($canonical_source_type) && !empty($similar_files['html'])) ? 'html' : $canonical_source_type;
    $canonical_source_type = (empty($canonical_source_type) && !empty($similar_files['pdf'])) ? 'pdf' : $canonical_source_type;
    $row->legacy_path = $similar_files[$canonical_source_type]['legacy_uri'];

    // Process the body.
    switch ($canonical_source_type) {
      case 'htm':
      case 'html':
        $system_file_path = $similar_files[$canonical_source_type]['uri'];
        $html = file_get_contents($system_file_path);
        $page = new AtrSimpleSpeechSourceParser(str_replace($this->baseDir, '', $system_file_path), $html);
        $row->body = $page->getBody();
        break;

      default:
        break;
    }
  }

  /**
   * {@inheritdoc}
   */
  public function complete($entity, $row) {
    parent::complete($entity, $row);

    // Set the organization field for each attached file.
    foreach ($entity->field_speech_attachment[LANGUAGE_NONE] as $delta => $file) {
      $file = file_load($file['fid']);
      $file->og_group_ref[LANGUAGE_NONE][0]['target_id'] = ATR_NID;
      file_save($file);
    }
  }

  /**
   * Builds an array of legacy urls for any attached files.
   *
   * @param array $files
   *   A file type keyed array of files containing a 'legacy_uri' element.
   *
   * @return array
   *   A flat array of legacy URIs.
   */
  public function getAttachedFileLegacyUrls($files) {
    $attach = array();
    if (!empty($files) && is_array($files)) {
      foreach ($files as $type => $file) {
        if (($type !== 'htm') && ($type !== 'html')) {
          // This file should be attached.
          $attach[] = $file['legacy_uri'];
        }
      }
    }
    return $attach;
  }

  /**
   * Builds a staff_profile node if it does not already exist.
   *
   * @param string $speaker
   *   The string used as the speaker's name.
   * @param string $speaker_role
   *   The speaker's job title.
   * @param int $og_id
   *   (optional). The organic group id for the speaker. Defaults to NULL.
   *
   * @return int
   *   The nid of the staff_profile node if created or pre-existing.
   */
  public function createStaffProfile($speaker, $speaker_role, $og_id = NULL) {
    // See if the profile already exists.
    $nid = $this->lookupMostRecentNodeByFieldValue('staff_profile', 'field_staff_name', $speaker);
    if (empty($nid)) {
      // Build the staff_profile.
      $node = new StdClass();
      $node->type = 'staff_profile';
      $node->language = LANGUAGE_NONE;
      node_object_prepare($node);
      $node->uid = 1;
      $node->title = $speaker;
      $node->field_staff_position[$node->language][0]['value'] = $speaker_role;
      $node->field_staff_name[$node->language][0]['value'] = $speaker;
      if (!empty($og_id)) {
        $node->og_group_ref[$node->language][0]['target_id'] = $og_id;
      }

      $node->status = 1;
      $node->workbench_moderation_state_current = 'published';
      $node->workbench_moderation_state_new = 'published';
      $node = node_submit($node);
      node_save($node);
      doj_migration_complete_workbench_settings($node, NULL);
      $nid = $node->nid;
      $this->migrationMessage("Staff Profile Node: @nid for '@speaker' was created.", array('@nid' => $nid, '@speaker' => $speaker), WATCHDOG_NOTICE, 1);
    }

    return $nid;
  }

  /**
   * Looks up the most recent published unarchived node by name by field value.
   *
   * @param string $node_type
   *   The node bundle to look within.
   * @param string $field_name
   *   The machine name of the field whose value being searched for.
   * @param string $lookup_value
   *   The value that is being searched for.
   *
   * @return int
   *   The nid of the most recent node if found, NULL if none was found.
   */
  public function lookupMostRecentNodeByFieldValue($node_type, $field_name, $lookup_value) {
    $nid = NULL;
    if (!empty($node_type) && !empty($field_name) && !empty($lookup_value)) {
      $query = new EntityFieldQuery();
      $query->entityCondition('entity_type', 'node')
        ->entityCondition('bundle', $node_type)
        ->fieldCondition($field_name, 'value', $lookup_value, '=')
        ->fieldCondition("field_archived", "value", 0, '=')
        ->propertyCondition('status', 1)
        ->propertyOrderBy('created', 'DESC');
      $results = $query->execute();
      if (!empty($results['node'])) {
        $nids = array_keys($results['node']);
        if (!empty($nids)) {
          // At least one staff_profile exists, so set the most recent one.
          $nid = $nids[0];
        }
      }

    }
    else {
      throw new MigrateException("lookupMostRecentNodeByFieldValue('$node_type', '$field_name', '$lookup_value') can not be executed with empty params.");
    }
    return $nid;
  }
}

/**
 * This sourceparser is used parse the pages containing the list of speeches.
 */
class AtrSpeechListSourceParser extends NGSpeechSourceParser {
  /**
   * Clean the html right before pulling the body.
   */
  protected function cleanHtml() {
    parent::cleanHtml();
  }
}

/**
 * This sourceparser is used parse the actual speech.
 */
class AtrSimpleSpeechSourceParser extends SourceParser {
  /**
   * Clean the html right before pulling the body.
   */
  protected function cleanHtml() {
    parent::cleanHtml();
    HtmlCleanUp::matchTextRemoveElement($this->queryPath, 'table', 'This document is available in');
    HtmlCleanUp::matchTextRemoveElement($this->queryPath, 'b', 'DEPARTMENT OF JUSTICE');
    $this->queryPath->find('font')->removeAttr('size')->removeAttr('face');
    $this->queryPath->find('table table')->first()->remove();

    // Selectors to remove.
    $selectors = array(
      'img[alt="U.S. Department of Justice Seal"]',
    );
    HtmlCleanUp::removeElements($this->queryPath, $selectors);
    HtmlCleanUp::removeTableBackgrounds($this->queryPath);
  }

  /**
   * Required by abstract.  Empty as no Obtainers are needed.
   */
  public function setDefaultObtainersInfo() {

  }

}

/**
 * Obtainer for body property for /usao-sdny speech.
 *
 * @package doj_migration
 * @subpackage usao-sdny
 * @see Obtainer.api.php
 */
class AtrSpeechObtainBody extends ObtainBody {
  /**
   * {@inheritdoc}
   */
  public static function cleanString($text) {
    $text = parent::cleanString($text);

    return $text;
  }
}

/**
 * Parses a single case year page into multiple cases.
 *
 * @package doj_migration
 * @subpackage core
 */
class AtrSpeechChunkParser extends JusticeChunkParser {

  /**
   * Sets $this->chunks.
   */
  public function setChunks() {
    // The pattern is well established on ATR Yearly speech pages .
    // @see http://www.justice.gov/atr/public/speeches/speech-2008.html
    $this->chunks = $this->sourceParser->queryPath->find('.atr-bottom-left p a');
    if (!$this->chunks || !$this->chunks->text()) {
      $this->chunks = array();
      $this->chunkparserMessage('Could not find chunks for JusticeChunkParser child class @class_name.', array('@class_name' => get_class()), WATCHDOG_ERROR);
    }
  }

  /**
   * {@inheritdoc}
   */
  public function getChunkIDs() {
    $chunks = $this->getChunks();
    $ids = array();
    $this->chunkparserMessage("Processing @chunkcount chunks.", array('@chunkcount' => $chunks->count()), FALSE);

    foreach ($chunks as $chunk) {
      // The ID of each speech will be the uri of the link to a speech file.
      $id = trim($chunk->attr('href'));

      // Ignore any '/press_releases/' paths.
      // Only add complete ids.
      if (!empty($id) && stripos($id, '/press_releases/') === FALSE) {
        $ids[] = $id;
      }
    }
    $this->chunkparserMessage("Created @idcount chunk IDs.", array('@idcount' => count($ids)), FALSE, 2);

    return $ids;
  }

  /**
   * {@inheritdoc}
   */
  public function getChunk($chunk_id) {
    $error_data = 'Data could not be migrated';
    $chunks = $this->getChunks();
    $this->chunkparserMessage("Getting chunk @chunkid .", array('@chunkid' => $chunk_id), FALSE);

    if (!$chunks) {
      watchdog('doj_migration', 'No data chunks were found. Cannot search for chunk @chunk_id', array('@chunk_id' => $chunk_id), WATCHDOG_ERROR);
      return $error_data;
    }
    $chunk = $chunks->find("a[href='$chunk_id']");

    if (!$chunk->innerHtml()) {
      watchdog('doj_migration', 'Data could not be migrated for chunk "@chunk_id"', array('@chunk_id' => $chunk_id), WATCHDOG_ERROR);
      return $error_data;
    }
    else {
      return $chunk;
    }
  }
}

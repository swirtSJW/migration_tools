<?php

/**
 * @file
 * Defines migration classes for Antitrust Division.
 */

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

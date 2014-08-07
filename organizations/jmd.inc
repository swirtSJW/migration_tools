<?php

/**
 * @file
 * Justice Management Division JMD.
 *
 * Defines migration classes for the JMD section of justice.gov.
 */

/**
 * Migrates .html files from /jmd to page nodes.
 *
 * @package doj_migration
 * @subpackage jmd
 */
class JmdPageMigration extends JusticeHtmlToPageMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {
    $json_dirs = '["jmd\/hr","jmd\/hr\/docs","jmd\/hr\/hrorder","jmd\/2010summary\/html","jmd\/ocio","jmd\/2011summary\/html","jmd\/eeos","jmd\/foia","jmd\/2013summary\/html","jmd\/2012summary\/html","jmd","jmd\/publications","jmd\/2009summary\/html","jmd\/ethics\/docs","jmd\/ethics","jmd\/ethics\/memo","jmd\/ls","jmd\/mps","jmd\/mps\/manual","jmd\/mps\/manual\/orgcharts","jmd\/mps\/manual\/maps","jmd\/2010summary","jmd\/2009justification","jmd\/2009justification\/exhibit300","jmd\/dcm","jmd\/2011summary","jmd\/pss","jmd\/2011factsheets","jmd\/eeos\/eo_reports","jmd\/eeos\/eo_reports\/2011","jmd\/eeos\/eo_reports\/2010","jmd\/eeos\/eo_reports\/2009","jmd\/eeos\/eo_reports\/2012","jmd\/2010justification","jmd\/2010justification\/exhibit300","jmd\/orginfo","jmd\/alo","jmd\/2013summary","jmd\/ccre","jmd\/2014summary","jmd\/2014summary\/html","jmd\/2013factsheets","jmd\/2012summary","jmd\/2009factsheets","jmd\/2015factsheets","jmd\/2012justification","jmd\/2012justification\/exhibit300","jmd\/2010factsheets","jmd\/2012factsheets","jmd\/2013justification","jmd\/2013justification\/exhibit300","jmd\/2015summary","jmd\/2009summary","jmd\/strategic2014-2018","jmd\/strategic2014-2018\/bk-dev\/strategic2014-2018","jmd\/ls\/legislative_histories\/pl103-141","jmd\/ls\/legislative_histories\/pl89-506","jmd\/ls\/legislative_histories\/pl100-700","jmd\/ls\/legislative_histories\/pl104-192","jmd\/ls\/legislative_histories\/pl106-110","jmd\/ls\/legislative_histories\/pl87-664","jmd\/ls\/legislative_histories\/pl79-404","jmd\/ls\/legislative_histories\/pl106-523","jmd\/ls\/legislative_histories\/pl106-547","jmd\/ls\/legislative_histories\/pl104-155","jmd\/ls\/legislative_histories\/pl107-119","jmd\/ls\/legislative_histories","jmd\/ls\/legislative_histories\/pl106-274","jmd\/ls\/legislative_histories\/pl100-346","jmd\/ls\/legislative_histories\/pl41-97","jmd\/ls\/legislative_histories\/pl93-528","jmd\/ls\/legislative_histories\/pl102-519","jmd\/ls\/legislative_histories\/pl65-322","jmd\/ls\/legislative_histories\/pl106-260","jmd\/ls\/legislative_histories\/pl105-184","jmd\/ls\/legislative_histories\/pl103-412","jmd\/ls\/legislative_histories\/pl108-191","jmd\/ls\/legislative_histories\/pl99-562","jmd\/ls\/legislative_histories\/pl89-487","jmd\/ls\/legislative_histories\/pl99-508","jmd\/ls\/legislative_histories\/pl106-185","jmd\/ls\/legislative_histories\/pl103-218","jmd\/ls\/legislative_histories\/pl108-136","jmd\/ls\/legislative_histories\/pl105-374","jmd\/2014factsheets","jmd\/2014justification","jmd\/2014justification\/exhibit300","jmd\/costreim","jmd\/2011justification","jmd\/2011justification\/exhibit300","jmd\/2015justification","jmd\/2015justification\/exhibit300"]';

    // Define source directories.
    $source_dirs = drupal_json_decode($json_dirs);

    $options = array('recurse' => FALSE);

    // Parent constructor will set $this->source, $this->destination, and
    // $this->map.
    parent::__construct($arguments, $source_dirs, $options);
    $this->dependencies = array('Organization');
    $this->description = t('Migrates pages from Justice Management Division section.');
    $this->addFieldMapping('og_group_ref')->defaultValue('jmd')->sourceMigration('Organization');
  }

  /**
   * {@inheritdoc}
   */
  public function prepareRow($row) {
    parent::prepareRow($row);

    // Clean up some titles.
    $title = $row->title;
    if (substr_count($title, "DOJ:") || substr_count($title, "DOJ :") || substr_count($title, "MPS:")) {
      $title = str_replace("DOJ:", "", $title);
      $title = str_replace("USDOJ:", "", $title);
      $title = str_replace("JMD:", "", $title);
      $title = str_replace("DOJ :", "", $title);
      $title = str_replace("USDOJ :", "", $title);
      $title = str_replace("JMD :", "", $title);
      $title = str_replace("MPS:", "", $title);
    }

    // Some of this titles are inside of the first h2 tag.
    if (empty($title)) {
      $query_body = htmlqp($row->field_page_body);
      $elements = $query_body->find("h2");
      foreach ($elements as $element) {
        $title = $element->text();
        break;
      }
    }

    // Some others are inside of a strong tag.
    if (empty($title)) {
      $query_body = htmlqp($row->field_page_body);
      $elements = $query_body->find("p>strong");
      foreach ($elements as $element) {
        $title = $element->text();
        break;
      }
    }

    $row->title = $title;
  }
}

/**
 * Migrates files (*.pdf) from /jmd.
 *
 * @package doj_migration
 * @subpackage jmd
 */
class JmdFileMigration extends JusticeDeployableBinaryFileMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {
    $this->description = t('Migrates non-image files from the jmd subdirectory.');
    $this->dependencies = array('Organization');

    $json_dirs = '["jmd","jmd\/hr","jmd\/hr\/docs","jmd\/hr\/hrorder\/PDF","jmd\/2010summary\/pdf","jmd\/ocio","jmd\/ocio\/2014itplan","jmd\/federal-program-inventory","jmd\/2009justification\/exhibit300","jmd\/2009justification\/pdf","jmd\/dcm","jmd\/2011summary\/pdf","jmd\/pss","jmd\/pia","jmd\/2011factsheets\/pdf","jmd\/eeos","jmd\/eeos\/eo_reports","jmd\/eeos\/eo_reports\/2011","jmd\/eeos\/eo_reports\/2010","jmd\/eeos\/eo_reports\/2009","jmd\/eeos\/eo_reports\/2014","jmd\/eeos\/eo_reports\/2013\/fy13-4thqtr","jmd\/eeos\/eo_reports\/2012","jmd\/eeos\/pdf","jmd\/2010justification\/exhibit300","jmd\/2010justification\/pdf","jmd\/orginfo\/images","jmd\/alo","jmd\/alo\/nov10","jmd\/alo\/may11","jmd\/alo\/may10","jmd\/2013summary\/pdf","jmd\/ccre","jmd\/ccre\/pdf","jmd\/2014summary\/pdf","jmd\/2013factsheets","jmd\/2012summary\/pdf","jmd\/2009factsheets\/pdf","jmd\/2015factsheets","jmd\/2012justification\/exhibit300","jmd\/2012justification\/pdf","jmd\/2010factsheets\/pdf","jmd\/2012factsheets\/docs","jmd\/2013justification\/exhibit300","jmd\/2013justification\/pdf","jmd\/2015summary\/pdf","jmd\/publications","jmd\/2009summary\/pdf","jmd\/strategic2014-2018\/bk-dev\/strategic2014-2018","jmd\/strategic2014-2018","jmd\/ethics\/docs","jmd\/ethics\/memo","jmd\/ethics","jmd\/ethics\/pdf","jmd\/ls","jmd\/ls\/legislative_histories\/pl103-141","jmd\/ls\/legislative_histories\/pl89-506","jmd\/ls\/legislative_histories\/pl100-700","jmd\/ls\/legislative_histories\/pl104-192","jmd\/ls\/legislative_histories\/pl106-110","jmd\/ls\/legislative_histories\/pl87-664","jmd\/ls\/legislative_histories\/pl79-404","jmd\/ls\/legislative_histories\/pl106-523","jmd\/ls\/legislative_histories\/pl106-547","jmd\/ls\/legislative_histories\/pl104-155","jmd\/ls\/legislative_histories\/pl107-119","jmd\/ls\/legislative_histories\/pl106-274","jmd\/ls\/legislative_histories\/pl100-346","jmd\/ls\/legislative_histories\/pl41-97","jmd\/ls\/legislative_histories\/pl93-528","jmd\/ls\/legislative_histories\/pl102-519","jmd\/ls\/legislative_histories\/pl65-322","jmd\/ls\/legislative_histories\/pl106-260","jmd\/ls\/legislative_histories\/pl105-184","jmd\/ls\/legislative_histories\/pl103-412","jmd\/ls\/legislative_histories\/pl108-191","jmd\/ls\/legislative_histories\/pl99-562","jmd\/ls\/legislative_histories\/pl89-487","jmd\/ls\/legislative_histories\/pl99-508","jmd\/ls\/legislative_histories\/pl106-185","jmd\/ls\/legislative_histories\/pl103-218","jmd\/ls\/legislative_histories\/pl108-136","jmd\/ls\/legislative_histories\/pl105-374","jmd\/2014factsheets","jmd\/2014justification\/exhibit300","jmd\/2014justification\/pdf","jmd\/costreim","jmd\/2011justification\/exhibit300","jmd\/2011justification\/pdf","jmd\/mps\/manual\/orgcharts","jmd\/2015justification\/exhibit300","jmd\/2015justification\/pdf","jmd\/2009justification\/office","jmd\/2010justification\/office","jmd\/2012justification\/office","jmd\/2013justification\/office","jmd\/2014justification\/office","jmd\/2011justification\/office","jmd\/2015justification\/office"]';

    // Match .pdf files only.
    $source_dirs = drupal_json_decode($json_dirs);
    $regex = '/.*\.(pdf|txt|rtf|doc|docx|xls|xlsx|csv|mp3|mp4|wpd|wp|qpw|xml|ppt|pptx)/';
    $dest_dir = 'public:///jmd/docs';

    // This will setup $this->map, $this->destination, and $this->source.
    // It will also add field mappings for file location and redirects, which
    // relies on prepareRow() defining $row->filepath and $row->legacy_path.
    parent::__construct($arguments, $source_dirs, $dest_dir, $regex);
  }

  /**
   * {@inheritdoc}
   */
  public function getOrganizationAbbreviation() {
    return "jmd";
  }
}

<?php

/**
 * @file
 * OIP.
 *
 * Defines migration classes for the OIP section of justice.gov.
 */

/**
 * These files should reside in wordpress://, which is generally
 * sites/default/private/wordpress.
 */
define('OIP_FOIA_POST_WORDPRESS_SOURCE', 'thefoiapost.wordpress.2014-07-10.xml');
define('OIP_COURT_WORDPRESS_SOURCE', 'courtdecisions.wordpress.2014-07-10.xml');


/**
 * Class OipFoiaPostMigration.
 *
 * @package doj_migration
 * @subpackage oip
 */
class OipFoiaPostMigration extends JusticeWordpressBlogEntry {
  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {
    $arguments['filename'] = doj_migrate_wordpress_prepare_file(OIP_FOIA_POST_WORDPRESS_SOURCE);
    $arguments['author_migration'] = "OipFoiaPostAuthor";
    parent::__construct($arguments);
    $this->addHardDependencies(array("OipFoiaPostCategory"));

    $this->addFieldMapping("field_blog_topic", 'category_value');
    $this->addFieldMapping("field_blog_date", 'pubDate');

    // Let's get the right taxonomy_term to set as default.
    $oip = "Office of Information Policy";
    $tts = taxonomy_get_term_by_name($oip, "component");
    $tt = reset($tts);
    if (isset($tt)) {
      $this->addFieldMapping("field_blog_component")->defaultValue($tt->name);
    }
    else {
      throw new Exception("Taxonomy Term '{$oip}' does not exist. Create it and re-run the migration");
    }
  }

  /**
   * {@inheritdoc}
   */
  public function prepareRow($row) {
    parent::prepareRow($row);
    // Don't add components as topics.
    if (is_array($row->category_value)) {
      foreach ($row->category_value as $key => $term) {
        if (taxonomy_get_term_by_name($term, "component")) {
          unset($row->category_value[$key]);
        }
      }
    }
  }
}

/**
 * Class OipFoiaPostAuthorMigration
 *
 * @package doj_migration
 * @subpackage oip
 */
class OipFoiaPostAuthorMigration extends JusticeWordpressAuthor {
  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {
    $arguments['filename'] = doj_migrate_wordpress_prepare_file(OIP_FOIA_POST_WORDPRESS_SOURCE);
    parent::__construct($arguments);
  }
}

/**
 * Class OipFoiaPostCategoryMigration.
 *
 * @package doj_migration
 * @subpackage oip
 */
class OipFoiaPostCategoryMigration extends JusticeWordpressCategory {
  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {
    $arguments['filename'] = doj_migrate_wordpress_prepare_file(OIP_FOIA_POST_WORDPRESS_SOURCE);
    parent::__construct($arguments);
  }
}

/**
 * Migrates .html files from /olc to page nodes.
 *
 * @package doj_migration
 * @subpackage olc
 */
class OipFoiaUpdateBlogEntryMigration extends JusticeHtmlToBlogEntryMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {
    // Define source directories.
    $source_dirs = array(
      "oip/foia_updates/Vol_I_1",
      "oip/foia_updates/Vol_I_2",
      "oip/foia_updates/Vol_I_3",
      "oip/foia_updates/Vol_I_4",
      "oip/foia_updates/Vol_II_1",
      "oip/foia_updates/Vol_II_2",
      "oip/foia_updates/Vol_II_3",
      "oip/foia_updates/Vol_II_4",
      "oip/foia_updates/Vol_III_1",
      "oip/foia_updates/Vol_III_2",
      "oip/foia_updates/Vol_III_3",
      "oip/foia_updates/Vol_III_4",
      "oip/foia_updates/Vol_IV_1",
      "oip/foia_updates/Vol_IV_2",
      "oip/foia_updates/Vol_IV_3",
      "oip/foia_updates/Vol_IV_4",
      "oip/foia_updates/Vol_V_1",
      "oip/foia_updates/Vol_V_2",
      "oip/foia_updates/Vol_V_3",
      "oip/foia_updates/Vol_V_4",
      "oip/foia_updates/Vol_VI_1",
      "oip/foia_updates/Vol_VI_2",
      "oip/foia_updates/Vol_VI_3",
      "oip/foia_updates/Vol_VI_4",
      "oip/foia_updates/Vol_VII_1",
      "oip/foia_updates/Vol_VII_2",
      "oip/foia_updates/Vol_VII_3",
      "oip/foia_updates/Vol_VII_4",
      "oip/foia_updates/Vol_VIII_1",
      "oip/foia_updates/Vol_VIII_2",
      "oip/foia_updates/Vol_VIII_3",
      "oip/foia_updates/Vol_IX_1",
      "oip/foia_updates/Vol_IX_2",
      "oip/foia_updates/Vol_IX_3",
      "oip/foia_updates/Vol_IX_4",
      "oip/foia_updates/Vol_X_1",
      "oip/foia_updates/Vol_X_2",
      "oip/foia_updates/Vol_X_3",
      "oip/foia_updates/Vol_X_4",
      "oip/foia_updates/Vol_XI_1",
      "oip/foia_updates/Vol_XI_2",
      "oip/foia_updates/Vol_XI_3",
      "oip/foia_updates/Vol_XII_1",
      "oip/foia_updates/Vol_XII_2",
      "oip/foia_updates/Vol_XII_3",
      "oip/foia_updates/Vol_XII_4",
      "oip/foia_updates/Vol_XIII_1",
      "oip/foia_updates/Vol_XIII_2",
      "oip/foia_updates/Vol_XIII_3",
      "oip/foia_updates/Vol_XIII_4",
      "oip/foia_updates/Vol_XIV_1",
      "oip/foia_updates/Vol_XIV_2",
      "oip/foia_updates/Vol_XIV_3",
      "oip/foia_updates/Vol_XV_1",
      "oip/foia_updates/Vol_XV_2",
      "oip/foia_updates/Vol_XV_3",
      "oip/foia_updates/Vol_XV_4",
      "oip/foia_updates/Vol_XVI_1",
      "oip/foia_updates/Vol_XVI_2",
      "oip/foia_updates/Vol_XVI_3",
      "oip/foia_updates/Vol_XVII_1",
      "oip/foia_updates/Vol_XVII_2",
      "oip/foia_updates/Vol_XVII_3",
      "oip/foia_updates/Vol_XVII_4",
      "oip/foia_updates/Vol_XVIII_1",
      "oip/foia_updates/Vol_XVIII_2",
      "oip/foia_updates/Vol_XVIII_3",
      "oip/foia_updates/Vol_XVIII_4",
      "oip/foia_updates/Vol_XIX_1",
      "oip/foia_updates/Vol_XIX_2",
      "oip/foia_updates/Vol_XIX_3",
      "oip/foia_updates/Vol_XIX_4",
      "oip/foia_updates/Vol_XX_1",
    );

    $options = array('recurse' => FALSE);

    // Parent constructor will set $this->source, $this->destination, and
    // $this->map.
    parent::__construct($arguments, $source_dirs, NULL, $options);
    $this->dependencies = array('Organization');
    $this->description = t('Migrates pages from OIP FOIA Update section.');
    $this->addFieldMapping('og_group_ref')->defaultValue('oip')->sourceMigration('Organization');

    // Let's get the right taxonomy_term to set as default.
    $terms = array(
      "FOIA Update" => "field_blog_topic",
      "Office of Information Policy" => "field_blog_component",
    );

    foreach ($terms as $term_name => $field_name) {
      $vocab = "topic";
      if ($field_name == "field_blog_component") {
        $vocab = "component";
      }
      $tts = taxonomy_get_term_by_name($term_name, $vocab);
      $tt = reset($tts);
      if ($tt) {
        $this->addFieldMapping($field_name)->defaultValue($tt->name);
      }
      else {
        throw new Exception("Taxonomy Term '{$term_name}' does not exist. Create it and re-run the migration");
      }
    }
  }

  /**
   * {@inheritdoc}
   */
  public function prepareRow(stdClass $row) {

    // Replace <h4> tags with <strong> tags.
    $source_parser = new SourceParser($row->url_path, $row->filedata);
    $source_parser->changeElementTag(array('h4' => 'strong'));

    parent::prepareRow($row, $source_parser);
    $row->field_blog_date = $this->getDateFromBody($row->field_blog_body);

    // Remove blank lines. This removes only an entire line that is blank,
    // not all \n or \r instances.
    // @see http://stackoverflow.com/a/709684
    $row->field_blog_body = preg_replace("/(^[\r\n]*|[\r\n]+)[\s\t]*[\r\n]+/", "\n", $row->field_blog_body);

    // Remove all \r and \n instances.
    $row->field_blog_body = preg_replace("/\r|\n/", " ", $row->field_blog_body);
  }

  /**
   * Find years (4 digits together) and return the date January 1, year.
   *
   * @param string $body
   *   String with dates.
   *
   * @return string
   *   A date formatted correctly to work with date field migrations.
   */
  protected function getDateFromBody($body) {
    preg_match('/(\d{4})/i', $body, $matches);
    $date = "1946-2-14 00:00:00";
    if (!empty($matches)) {
      $year = $matches[0];
      $year = trim($year);
      $date = "{$year}-1-1 00:00:00";
    }
    return $date;
  }
}

/**
 * Class OipCourtMigration.
 *
 * @package doj_migration
 * @subpackage oip
 */
class OipCourtMigration extends JusticeWordpressCourtDecision {
  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {
    $arguments['filename'] = doj_migrate_wordpress_prepare_file(OIP_COURT_WORDPRESS_SOURCE);
    $arguments['author_migration'] = "OipCourtDecisionAuthor";
    parent::__construct($arguments);
    $this->addHardDependencies(array("OipCourtDecisionCategory"));

    $this->addFieldMapping("field_cd_topic", 'category_value');
    $this->addFieldMapping("field_cd_date", 'field_cd_date');
  }

  /**
   * {@inheritdoc}
   */
  public function prepareRow($row) {
    parent::prepareRow($row);
    // Extract the post date.
    preg_match('/[A-Za-z]{3,9}(\.)? (\d{1,2}), \d{4}/', $row->title, $results);
    if (!empty($results)) {
      $date = $results[0];
      $row->field_cd_date = $date;
    }
  }
}

/**
 * Class OipCourtAuthorMigration
 *
 * @package doj_migration
 * @subpackage oip
 */
class OipCourtAuthorMigration extends JusticeWordpressAuthor {
  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {
    $arguments['filename'] = doj_migrate_wordpress_prepare_file(OIP_COURT_WORDPRESS_SOURCE);
    parent::__construct($arguments);
  }
}

/**
 * Class OipCourtCategoryMigration.
 *
 * @package doj_migration
 * @subpackage oip
 */
class OipCourtCategoryMigration extends JusticeWordpressCategory {
  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {
    $arguments['category_vocabulary'] = "court_decision_topic";
    $arguments['filename'] = doj_migrate_wordpress_prepare_file(OIP_COURT_WORDPRESS_SOURCE);
    parent::__construct($arguments);
  }
}

/**
 * Migrates .html files from /oip to page nodes.
 *
 * @package doj_migration
 * @subpackage oip
 */
class OipPageMigration extends JusticeHtmlToPageMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {
    // Define source directories.
    $source_dirs = array(
      'oip',
      'oip/annual_report',
      'oip/annual_report/2000',
      'oip/annual_report/2001',
      'oip/annual_report/2002',
      'oip/annual_report/2003',
      'oip/annual_report/2004',
      'oip/annual_report/2005',
      'oip/annual_report/2006',
      'oip/annual_report/2007',
      'oip/annual_report/2008',
      'oip/annual_report/2009',
      'oip/annual_report/2010',
      'oip/annual_report/2011',
      'oip/annual_report/2013',
      'oip/courtdecisions',
      'oip/csv',
      'oip/cy09',
      'oip/cy10',
      'oip/diversity',
      'oip/docs',
      'oip/files',
      'oip/foia-guide13',
      'oip/foia-guide14',
      'oip/foia-guide07',
      'oip/foia-guide09',
      'oip/foia-updates',
      'oip/foiacontact',
      'oip/forms',
      'oip/images',
      'oip/lc-rpt-2011',
      'oip/lc-rpt-2012',
      'oip/lc-rpt-2013',
      'oip/leg-history',
      'oip/niem',
      'oip/readingrooms',
      'oip/treenav',
      'oip/xls',
    );
    $options = array('recurse' => FALSE);

    // Parent constructor will set $this->source, $this->destination, and
    // $this->map.
    parent::__construct($arguments, $source_dirs, NULL, $options);
    $this->dependencies = array('Organization');
    $this->description = t('Migrates pages from Office of Information Policy section.');
    $this->addFieldMapping('og_group_ref')->defaultValue('oip')->sourceMigration('Organization');
  }
}

/**
 * Migrates .html files from /oip to page nodes.
 *
 * @package doj_migration
 * @subpackage oip
 */
class OipFoiaPostBlogEntryMigration extends JusticeHtmlToBlogEntryMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {
    // Define source directories.
    $source_dirs = array(
      'oip/foiapost',
    );
    $options = array('recurse' => FALSE);

    // Parent constructor will set $this->source, $this->destination, and
    // $this->map.
    parent::__construct($arguments, $source_dirs, NULL, $options);
    $this->dependencies = array('Organization');
    $this->description = t('Migrates pages from Office of Information Policy section.');
    $this->addFieldMapping('og_group_ref')->defaultValue('oip')->sourceMigration('Organization');

    // Let's get the right taxonomy_term to set as default.
    $terms = array(
      "FOIA Post" => "field_blog_topic",
      "Office of Information Policy" => "field_blog_component",
    );

    foreach ($terms as $term_name => $field_name) {
      $vocab = "topic";
      if ($field_name == "field_blog_component") {
        $vocab = "component";
      }
      $tts = taxonomy_get_term_by_name($term_name, $vocab);
      $tt = reset($tts);
      if ($tt) {
        $this->addFieldMapping($field_name)->defaultValue($tt->name);
      }
      else {
        throw new Exception("Taxonomy Term '{$term_name}' does not exist. Create it and re-run the migration");
      }
    }
  }

  /**
   * {@inheritdoc}
   */
  public function prepareRow($row) {

    // Create a new SourceParser to handle HTML content.
    $source_parser = new SourceParser($row->url_path, $row->filedata);

    // Remove table containing header image.
    $source_parser->queryPath
      ->find('img[src="//foiapost-1.jpg"], img[src="//foiapost-2.jpg"]')
      ->parent('table')
      ->replaceWith('<img src="/sites/default/files/oip/images/foiapost.jpg" class="foiapost-banner" alt="FOIA Post" />');

    // Remove <font> tags.
    $font_tags = $source_parser->queryPath->find('font');
    foreach ($font_tags as $font_tag) {
      $font_tag->textBefore($font_tag->text());
      $font_tag->remove();
    }

    parent::prepareRow($row, $source_parser);

    // Extract the post date.
    if (preg_match('/posted (\d{1,2}\/\d{1,2}\/\d{2,4})/', $row->field_blog_body, $results)) {
      $row->field_blog_date = end($results);
    }
    elseif (preg_match('/\d{1,2}\/\d{1,2}\/\d{2,4}/', $row->field_blog_body, $results)) {
      $row->field_blog_date = end($results);
    }
  }
}

/**
 * Migrates binary files from /oip.
 *
 * @package doj_migration
 * @subpackage oip
 */
class OipFileMigration extends JusticeDeployableBinaryFileMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {
    $this->description = t('Migrates binary files from the oip subdirectory.');
    $this->dependencies = array('Organization');

    // Match binary files only.
    $source_dirs = array(
      'oip/csv',
      'oip/xls',
      'oip/foia-guide14',
      'oip/lc-rpt-2012',
      'oip/docs',
      'oip/foiapost',
      'oip/lc-rpt-2013',
      'oip/foia-guide13',
      'oip/readingrooms',
      'oip/readingrooms',
      'oip/foia_guide09',
      'oip/annual_report/2007',
      'oip/annual_report/2011',
      'oip/annual_report/2010',
      'oip/annual_report/2009',
      'oip/annual_report/2008',
      'oip/annual_report/2013',
      'oip/annual_report/2012',
      'oip/foiacontact',
      'oip/forms',
      'oip/lc-rpt-2011',
      'oip/cy09',
      'oip/cy10',
      'oip/foia_guide07',
      'oip/leg-history',
      'oip',
      'oip/niem',
    );

    $regex = '/.*\.(pdf|txt|rtf|doc|docx|xls|xlsx|csv|mp3|mp4|wpd|wp|qpw|xml|ppt|pptx)/i';
    $dest_dir = 'public:///oip/docs';

    // This will setup $this->map, $this->destination, and $this->source.
    // It will also add field mappings for file location and redirects, which
    // relies on prepareRow() defining $row->filepath and $row->legacy_path.
    parent::__construct($arguments, $source_dirs, $dest_dir, $regex);
  }

  /**
   * {@inheritdoc}
   */
  public function getOrganizationAbbreviation() {
    return "oip";
  }
}

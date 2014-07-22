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
define('OIP_FOIA_UPDATE_WORDPRESS_SOURCE', 'thefoiapost.wordpress.2014-07-10.xml');
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
 * Class OipFoiaUpdateMigration.
 *
 * @package doj_migration
 * @subpackage oip
 */
class OipFoiaUpdateMigration extends JusticeWordpressBlogEntry {
  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {
    $arguments['filename'] = doj_migrate_wordpress_prepare_file(OIP_FOIA_UPDATE_WORDPRESS_SOURCE);
    $arguments['author_migration'] = "OipFoiaUpdateAuthor";
    parent::__construct($arguments);
    $this->addHardDependencies(array("OipFoiaUpdateCategory"));

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
}

/**
 * Class OipFoiaUpdateAuthorMigration
 *
 * @package doj_migration
 * @subpackage oip
 */
class OipFoiaUpdateAuthorMigration extends JusticeWordpressAuthor {
  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {
    $arguments['filename'] = doj_migrate_wordpress_prepare_file(OIP_FOIA_UPDATE_WORDPRESS_SOURCE);
    parent::__construct($arguments);
  }
}

/**
 * Class OipFoiaUpdateCategoryMigration.
 *
 * @package doj_migration
 * @subpackage oip
 */
class OipFoiaUpdateCategoryMigration extends JusticeWordpressCategory {
  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {
    $arguments['filename'] = doj_migrate_wordpress_prepare_file(OIP_FOIA_UPDATE_WORDPRESS_SOURCE);
    parent::__construct($arguments);
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
      'oip/foiapost',
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
    parent::__construct($arguments, $source_dirs, $options);
    $this->dependencies = array('Organization');
    $this->description = t('Migrates pages from Office of Information Policy section.');
    $this->addFieldMapping('og_group_ref')->defaultValue('oip')->sourceMigration('Organization');
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

    $regex = '/.*\.(pdf|txt|rtf|doc|docx|xls|xlsx|csv|mp3|mp4|wpd|wp|qpw|xml|ppt|pptx)/';
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

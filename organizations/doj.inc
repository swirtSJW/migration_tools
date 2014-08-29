<?php

/**
 * @file
 * Department of Justice.
 *
 * Defines migration classes for the DOJ section of justice.gov.
 */

/**
 * Migrates .html files from /crs to page nodes.
 *
 * @package doj_migration
 * @subpackage doj
 */
class DojPageMigration extends JusticeHtmlToPageMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {
    // Define source directories.
    $source_dirs = array(
      'about',
      'accessibility',
      'accomplishments',
      'actioncenter',
      'agencies',
      'business',
      'cjs',
      'data',
      'dec',
      'developer',
      'digitalstrategy',
      'employees',
      'healthcare',
      'iqpr',
      'joiningforces',
      'publications',
      'sitemap',

      'about.html',
      'a-z-index.html',
      'component.htm',
      'contact-us.html',
      'dojofficials.htm',
      'legalpolicies.htm',
      'opa-in-memoriam.htm',
      'privacy-file.htm',
      'rss.htm',
    );
    $options = array('recurse' => TRUE);

    // Parent constructor will set $this->source, $this->destination, and
    // $this->map.
    parent::__construct($arguments, $source_dirs, NULL, $options);
    $this->dependencies = array('Organization');
    $this->description = t('Migrates pages from main Department of Justice section.');
    // Hard coding in the nid of the DOJ group.
    $doj_nid = 752;
    $this->addFieldMapping('og_group_ref')->defaultValue($doj_nid);

    // Since this content belongs to DOJ but is not going into /doj/ then
    // pathauto must be overridden.  See prepareRow().
    $this->addFieldMapping('path', 'url_alias');

    // @FIXIT This setting is not getting saved. Setting it by
    // hand in JusticeHtmlNodeMigration prepare().
    $this->addFieldMapping('pathauto', FALSE);
  }

  /**
   * {@inheritdoc}
   */
  public function prepareRow($row) {
    // Have to set the the group_path_override before calling the parent
    // prepareRow or it will be too late and the alias will already be set.
    // Since the doj group is the root of the site, the group path is ''.
    $row->group_path_override = '';

    parent::prepareRow($row);
    $row->body = parent::dojMigrateStripCmsLegacyMarkup($row->body);

    // Check for title alterations needed if title is "Publications".
    if ($row->title == 'Publications') {
      // Grab a more meaningful title from the <title>.
      $row->title = $this->sourceParser->queryPath->find('title')->first()->text();
    }

  }
}


/**
 * Migrates non-image files from the doj content.
 *
 * @package doj_migration
 * @subpackage doj
 */
class DojFileMigration extends JusticeDeployableBinaryFileMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {
    $this->description = t('Migrates non-image files from the doj (remaining sections of version4 tarball).');
    $this->dependencies = array('Organization');

    // Match non-image files.
    $source_dirs = array(
      'accomplishments',
      'business/itss',
      'cjs/docs',
      'data',
      'employees',
      'healthcare/docs',
      'publications',
      'publications/waco',
    );
    $regex = '/.*\.(pdf|txt|rtf|doc|docx|xls|xlsx|csv|mp3|mp4|wpd|wp|qpw|xml|ppt|pptx|swf|flv)/';
    $dest_dir = 'public:///doj/docs';

    // This will setup $this->map, $this->destination, and $this->source.
    // It will also add field mappings for file location and redirects, which
    // relies on prepareRow() defining $row->filepath and $row->legacy_path.
    parent::__construct($arguments, $source_dirs, $dest_dir, $regex);
  }

  /**
   * {@inheritdoc}
   */
  public function getOrganizationAbbreviation() {
    return 'doj';
  }
}

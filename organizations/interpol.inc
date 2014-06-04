<?php

/**
 * @file
 * INTERPOL.
 *
 * Defines migration classes for the INTERPOL section of justice.gov.
 */

/**
 * Migrates .html files from /interpol-washington to page nodes.
 *
 * @package doj_migration
 * @subpackage interpol
 */
class InterpolPageMigration extends JusticeHtmlToPageMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {
    // Define source directories.
    $source_dirs = array(
      'interpol-washington',
    );
    // Assemble a regular expression of files to ignore.
    $nomask = '|usncb-rightcol\\.html';
    $options = array(
      'recurse' => FALSE,
      'nomask' => '/(\.\.?|CVS' . $nomask . ')$/',
    );

    // Parent constructor will set $this->source, $this->destination, and
    // $this->map.
    parent::__construct($arguments, $source_dirs, $options);
    $this->dependencies = array('Organization');
    $this->description = t('Migrates pages from INTERPOL section.');
    $this->addFieldMapping('og_group_ref')->defaultValue('interpol-washington')->sourceMigration('Organization');
  }
}

/**
 * Migrates .html files from /interpol-washington/news to press release nodes.
 *
 * @package doj_migration
 * @subpackage interpol
 */
class InterpolNewsMigration extends JusticeHtmlToPageMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {
    // Define source directories.
    $source_dirs = array(
      'interpol-washington/news',
    );
    $options = array(
      'recurse' => FALSE,
    );

    // Parent constructor will set $this->source, $this->destination, and
    // $this->map.
    parent::__construct($arguments, $source_dirs, $options);
    $this->destination = new MigrateDestinationNode('press_release');
    $this->dependencies = array('Organization');
    $this->description = t('Migrates pages from INTERPOL section.');
    $this->addFieldMapping('og_group_ref')->defaultValue('interpol-washington')->sourceMigration('Organization');
    $this->addFieldMapping('field_pr_date', 'field_pr_date');
    $this->addFieldMapping('field_pr_body', 'field_pr_body');
    $this->addFieldMapping('field_pr_body:format')->defaultValue('wysiwyg');
  }

  /**
   * {@inheritdoc}
   */
  public function prepareRow($row) {
    parent::prepareRow($row);

    // Create a new SourceParser to handle HTML content.
    $source_parser = new SourceParser($row->legacy_path, (string) $row->filedata, TRUE);

    // Extract title.
    if ($title_element = $source_parser->queryPath->find('.presscontenttitle')->text()) {
      $title = $title_element;
      // Clean string.
      $title = $source_parser->removeWhitespace($title);

      // Truncate title to max of 255 characters.
      if (strlen($title) > 255) {
        $title = substr($title, 0, 255);
      }
    }
    else {
      $title = $source_parser->getTitle();
    }

    $row->title = $title;

    // Map the date.
    $created = strtotime($source_parser->queryPath->find('.date')->text());
    $row->field_pr_date = $created;
    // Map the body.
    $row->field_pr_body = $source_parser->getBody();
  }

}


/**
 * Migrates binary files from /interpol-washington.
 *
 * @package doj_migration
 * @subpackage interpol
 */
class InterpolFileMigration extends JusticeDeployableBinaryFileMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {
    $this->description = t('Migrates binary files from the interpol-washington subdirectory.');
    $this->dependencies = array('Organization');

    // Match binary files only.
    $source_dirs = array(
      'interpol-washington',
      'interpol-washington/foia',
    );
    $regex = '/.*\.(pdf|txt|rtf|doc|docx|xls|xlsx|csv|mp3|mp4|wpd|wp|qpw|xml|ppt|pptx)/';
    $dest_dir = 'public:///interpol-washington/docs';

    // This will setup $this->map, $this->destination, and $this->source.
    // It will also add field mappings for file location and redirects, which
    // relies on prepareRow() defining $row->filepath and $row->legacy_path.
    parent::__construct($arguments, $source_dirs, $dest_dir, $regex);
  }

  /**
   * {@inheritdoc}
   */
  public function getOrganizationAbbreviation() {
    return "interpol-washington";
  }
}

<?php
/**
 * @file
 * Defines migration classes for ...
 */

class AtrGalleryMigration extends JusticeHtmlToGalleryMigration {

  /**
   * Constructor.
   */
  public function __construct($arguments) {
    // Define source directories.
    $source_dirs = array(
      'atr/public',
    );
    $options = array(
      'recurse' => TRUE,
    );
    $arguments['obtainer_methods'] = array(
      'title' => array(
        'findTitleTag',
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

class AtrGalleryImagesMigration extends JusticeHtmlToGalleryImagesMigration {

  /**
   * Constructor.
   */
  public function __construct($arguments) {
    // Define source directories.
    $source_dirs = array(
      'atr/public',
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

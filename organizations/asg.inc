<?php

/**
 * @file
 * Office of Associate Attorney General ASG.
 *
 * Defines migration classes for the ASG section of justice.gov.
 */

/**
 * Migrates .html files from /asg to page nodes.
 *
 * @package doj_migration
 * @subpackage asg
 */
class AsgPageMigration extends JusticeHtmlToPageMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {
    // Define source directories.
    $source_dirs = array(
      'asg',
      'asg/testimony/2009',
      'asg/testimony/2010',
      'asg/testimony',
      'asg/readingroom',

    );
    $options = array('recurse' => FALSE);

    // Parent constructor will set $this->source, $this->destination, and
    // $this->map.
    parent::__construct($arguments, $source_dirs, $options);
    $this->dependencies = array('Organization');
    $this->description = t('Office of the Associate Attorney General.');
    $this->addFieldMapping('og_group_ref')->defaultValue('asg')->sourceMigration('Organization');
  }
}

/**
 * Migrates files (*.pdf) from /asg.
 *
 * @package doj_migration
 * @subpackage asg
 */
class AsgFileMigration extends JusticeDeployableBinaryFileMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {
    $this->description = t('Migrates non-image files from the asg subdirectory.');
    $this->dependencies = array('Organization');

    // Match .pdf files only.
    $source_dirs = array(
      'asg',
      'asg/readingroom',
    );
    $regex = '/.*\.(pdf|txt|rtf|doc|docx|xls|xlsx|csv|mp3|mp4|wpd|wp|qpw|xml|ppt|pptx)/';
    $dest_dir = 'public:///asg/docs';

    // This will setup $this->map, $this->destination, and $this->source.
    // It will also add field mappings for file location and redirects, which
    // relies on prepareRow() defining $row->filepath and $row->legacy_path.
    parent::__construct($arguments, $source_dirs, $dest_dir, $regex);
  }

  /**
   * {@inheritdoc}
   */
  public function getOrganizationAbbreviation() {
    return "asg";
  }
}

/**
 * Migrates speeches from /asg.
 *
 * @package doj_migration
 * @subpackage asg
 */
class AsgSpeechMigration extends JusticeHtmlToSpeechMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {
    // Define source directories.
    $source_dirs = array(
      'asg/speeches/2008',
      'asg/speeches/2009',
      'asg/speeches/2010',
    );

    $options = array(
      'recurse' => FALSE,
      'nomask' => '/(\.\.?|CVS|.*-spanish.html?)$/',
    );

    // Parent constructor will set $this->source, $this->destination, and
    // $this->map.
    parent::__construct($arguments, $source_dirs, $options);
    $this->dependencies = array('Organization');
    $this->description = t('Office of the Associate Attorney General.');
    $this->addFieldMapping('og_group_ref')->defaultValue('asg')->sourceMigration('Organization');
  }

  /**
   * {@inheritdoc}
   */
  public function prepareRow($row, SourceParser $source_parser = NULL) {
    // Create a new SourceParser to handle HTML content.
    if (!$source_parser) {
      $source_parser = new SourceParser($row->url_path, $row->filedata);
    }

    // Get title from .presscontenttitle.
    $row->title = $source_parser->queryPath->find('.presscontenttitle')->first()->text();
    $row->title = mb_strimwidth($row->title, 0, 255, "...");
    $row->title = html_entity_decode($row->title, ENT_QUOTES, 'UTF-8');

    $body = '';
    $body = $source_parser->queryPath->find('em')->remove();
    $body = $source_parser->queryPath->find('.presscontenttext')->first()->html();

    $body = doj_migrate_strip_cmsopa_legacy_markup($body);

    $row->field_speech_body = $body;

    $row->field_speech_date = $source_parser->queryPath->find('.speechdate')->text();

    // This data is a mess.
    $location = '';
    $city = '';
    $state = '';

    $location = strtolower($source_parser->queryPath->find('.speechlocation')->text());

    list($city, $state) = explode(', ', $location);

    $state = strtoupper(strtr($state, array('.' => '')));

    if (strlen($state) > 2) {
      switch ($state) {
        case 'MASS':
          $state = 'MA';
          break;

        case 'WASH':
          $state = 'WA';
          break;

        case 'MICH':
          $state = 'MI';
          break;

        case 'MINN':
          $state = 'MN';
          break;

      }
    }

    $row->state = $state;
    $row->city = ucwords($city);
    parent::prepareRow($row, $source_parser);
  }
}

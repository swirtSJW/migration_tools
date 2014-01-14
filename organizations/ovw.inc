<?php

/**
 * @file
 * Defines migration classes for the Careers section of justice.gov.
 */

/**
 * Abstract intermediate class holding common settings.
 */
abstract class OvwMigration extends JusticeMigration {
  public $basicFormat;

  /**
   * {@inheritdoc}
   */
  public function __construct() {
    // We group all sub classes of this super class under the 'OVW' group,
    // and indicate that it should appear after the 'default' group in UI.
    parent::__construct(MigrateGroup::getInstance('ovw', array('default')));
  }
}

/**
 * Migrates calendars from OPA CMS calendarlist table for Careers section.
 */
class OvwSpeechesMigration extends CareersMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct() {
    parent::__construct();

    $this->description = t('Migrates calendars from OPA CMS calendarlist table for Careers section.');
    $this->map = new MigrateSQLMap($this->machineName,
      array(
        'style' => array('type' => 'varchar',
          'length' => 255,
          'not null' => TRUE,
          'description' => 'Topic ID',
        )
      ),
      MigrateDestinationTerm::getKeySchema()
    );

    // In this example, we're using tables that have been added to the existing
    // Drupal database but which are not Drupal tables. You can examine the
    // various tables (starting here with migrate_example_beer_topic) using a
    // database browser like phpMyAdmin.
    // First, we set up a query for this data. Note that by ordering on
    // style_parent, we guarantee root terms are migrated first, so the
    // parent_name mapping below will find that the parent exists.
    $query = db_select('migrate_example_beer_topic', 'met')
      ->fields('met', array('style', 'details', 'style_parent', 'region', 'hoppiness'))
      // This sort assures that parents are saved before children.
      ->orderBy('style_parent', 'ASC');

    // Create a MigrateSource object, which manages retrieving the input data.
    $this->source = new MigrateSourceSQL($query);

    // Set up our destination - terms in the migrate_example_beer_styles vocabulary
    $this->destination = new MigrateDestinationTerm('migrate_example_beer_styles');

    // Assign mappings TO destination fields FROM source fields. To discover
    // the names used in these calls, use the drush commands
    // drush migrate-fields-destination BeerTerm
    // drush migrate-fields-source BeerTerm
    $this->addFieldMapping('name', 'style');
    $this->addFieldMapping('description', 'details');

    // Documenting your mappings makes it easier for the whole team to see
    // exactly what the status is when developing a migration process.
    $this->addFieldMapping('parent_name', 'style_parent')
      ->description(t('The incoming style_parent field is the name of the term parent'));

    // Mappings are assigned issue groups, by which they are grouped on the
    // migration info page when the migrate_ui module is enabled. The default
    // is 'Done', indicating active mappings which need no attention. A
    // suggested practice is to use groups of:
    // Do Not Migrate (or DNM) to indicate source fields which are not being used,
    //  or destination fields not to be populated by migration.
    // Client Issues to indicate input from the client is needed to determine
    //  how a given field is to be migrated.
    // Implementor Issues to indicate that the client has provided all the
    //  necessary information, and now the implementor needs to complete the work.
    $this->addFieldMapping(NULL, 'hoppiness')
      ->description(t('This info will not be maintained in Drupal'))
      ->issueGroup(t('DNM'));

    // Open mapping issues can be assigned priorities (the default is
    // MigrateFieldMapping::ISSUE_PRIORITY_OK). If you're using an issue
    // tracking system, and have defined issuePattern (see BasicExampleMigration
    // above), you can specify a ticket/issue number in the system on the
    // mapping and migrate_ui will link directory to it.
    $this->addFieldMapping(NULL, 'region')
      ->description('Will a field be added to the vocabulary for this?')
      ->issueGroup(t('Client Issues'))
      ->issuePriority(MigrateFieldMapping::ISSUE_PRIORITY_MEDIUM)
      ->issueNumber(770064);

    // It is good practice to account for all source and destination fields
    // explicitly - this makes sure that everyone understands exactly what is
    // being migrated and what is not. Also, migrate_ui highlights unmapped
    // fields, or mappings involving fields not in the source and destination,
    // so if (for example) a new field is added to the destination field it's
    // immediately visible, and you can find out if anything needs to be
    // migrated into it.
    $this->addFieldMapping('format')
      ->issueGroup(t('DNM'));
    $this->addFieldMapping('weight')
      ->issueGroup(t('DNM'));
    $this->addFieldMapping('parent')
      ->issueGroup(t('DNM'));

    // We conditionally DNM these fields, so your field mappings will be clean
    // whether or not you have path and or pathauto enabled
    if (module_exists('path')) {
      $this->addFieldMapping('path')
        ->issueGroup(t('DNM'));
      if (module_exists('pathauto')) {
        $this->addFieldMapping('pathauto')
          ->issueGroup(t('DNM'));
      }
    }
  }
}


<?php

/**
 * @file
 * Contains classes for migrations from Wordpress.
 */

abstract class JusticeWordpressMigration extends WordPressMigration {
  public function __construct(array $arguments = array()) {
    parent::__construct($arguments);

    $this->map = new MigrateSQLMap($this->machineName,
      array(
        'author_login' => array(
          'type' => 'varchar',
          'length' => 255,
          'not null' => TRUE,
          'description' => 'Wordx`Press author username',
        )
      ),
      MigrateDestinationNode::getKeySchema()
    );

    $fields = array(
      'author_id' => 'Unique WordPress ID of the author',
      'author_login' => 'Username (for login) of the author',
      'author_email' => 'Author email address',
      'author_display_name' => 'Displayed author name',
      'author_first_name' => 'Author first name',
      'author_last_name' => 'Author last name',
    );

    // Construct the source and destination objects.
    $source_options = array(
      'reader_class' => 'WordPressXMLReader',
      'cache_counts' => TRUE,
    );
    $this->source = new WordPressSourceXML($this->wxrFile, '/rss/channel/wp:author',
      'wp:author_login', $fields, $source_options);
    $this->destination = new BlogImportNodeDestination('person');

    // The basic mappings
    $this->addFieldMapping('title', 'author_display_name');
    $this->addFieldMapping('status')
      ->defaultValue(NODE_PUBLISHED);
    $this->addFieldMapping('uid')
      ->defaultValue(1);

    // Unmapped destination fields
    $this->addUnmigratedDestinations(array());

    // Unmapped source fields
    $this->addUnmigratedSources(array());
  }
}

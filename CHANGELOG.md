migration_tools 7.x-2.x  ** - ** - ****
-----------------------------------------------
* Add Migration\Base class.
* Add sourcer class to load html files from a local directory
  https://www.drupal.org/node/2709651
* Add admin field for handling migration source location.
* Modified settings location to reside with other Migrate settings.
* Modified method of hiding the Migrate settings.
* PSR-4 autoloader added to autoload classes
* Renaming and moving of classes to support autoloading and namespacing.
* Stub for redirect detection.
* Destination URI validation.
* Add migration-tools-html-file-list drush utility command. 

migration_tools 7.x-1.x  April 14, 2016
-----------------------------------------------
The 1.x branch is no longer being maintained.  The 7.x-2.x branch is now active.
* Consolidate methods for cleaning node titles.
* Add CHANGELOG.md
* Moved URL and redirect related methods into UrlTools.inc


migration_tools 7.x-1.0-alpha2  before April 13, 2016
-----------------------------------------------
* Created project from pieces used to migrate other sites.  Things that are
  currently not connected to the module's code are in the examples directory.

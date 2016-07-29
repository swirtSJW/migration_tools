migration_tools 7.x-2.x  ** - ** - ****
-----------------------------------------------
* Issue #2774963: Make addSearch return the $this so that they can be chained.
* Issue #2773835: Add ObtainArray support to Obtainers


migration_tools 7.x-2.4  July 27, 2016
-----------------------------------------------
* Issue #2770953: Add function to remove empty tables.
* Issue #2772389 Add Modifier class to stack up html modifiers to run in cleanQueryPathHtml()
  in the SourceParser.


migration_tools 7.x-2.3  July 15, 2016
-----------------------------------------------
* Fix Error if using QueryPath version that does not support size()/count().
* Add findSelectorNSeparator to ObtainHtml.
* Add findSelector to ObtainHtml.
* Add reduceDuplicateBr() to StringTools.
* Removed duplicate internal Table cell plucker.
* Renamed pluckTableContents to pluckTableCellContents to make it more explicit.
* Renamed internal method pluckTableCell to extractTableCell so to keep it
  as an internal method not a plucker.
* Fix false positives on scanning for JS redirects.
* Issue #2755321: Create a set of source tools to handle migrating from an array of URLs.
* Add drush command mt-generate-redirects-list [filename] to read a list of urls
  and turn it into a list of redirects for any that were detected in html or
  server-side.
* Issue #2753779: Add file data to row properties
* Issue #2749377: Rewiting an empty href results in bad link
* Fixed bug where longdescription paths were not getting checked.
* Add javascript src rewriting and flash attribute rewriting.
* Adjust ObtainTitle to better remove whitespaces in title.
* Bugfix ObtainDate where a short date is wrongly rejected.
* Add more verbose output using: drush vset migration_tools_drush_debug 2
* Add alias reporting.
* Fix bug in Url::generateDestinationUriAlias().


migration_tools 7.x-2.2  May 31, 2016
-----------------------------------------------
* Move instance of SourceParser from $row to migration $this.
* Fix misnamed variable in link re-writing methods.
* Add QpHtml::removeComment()
* Move some basic cleaning calls into SourceParser\HtmlBase.


migration_tools 7.x-2.1  May 27, 2016
-----------------------------------------------
* Add stub class for Source\Url for when the source is live URLs.
* Minor improvements to Meta Redirect detection.
* Add redirect detection and handling to Migration\HtmlFileBase.
  Relocated some handling between Base and HtmlFileBase.
* Add CheckFor isSkipAndRedirect to allow for skipping and redirecting.
* Improvements to ObtainDate.
* Add OrganicGroups class for tools to handle OG issues.
* Adjust params on CheckFor isInPath isSkipFile to make them consistent in
  param order of needle, haystack.
* Add constructor to Url.php to create pathing object and some refactoring to
  use and support the new pathing object.
* Add Migration\HtmlFileBase class.
* Improved URL/URI rewriting method for page href, files and img src.
* Add Message::makeSummary()
* Align drush command terms.
* Improve html redirect destination verification.
* Add Message::makeSkip().
* Completed detection of html and javascript redirects.
* Connect Obtainer classes and add Obtainer\Job
* Add SourceParser classes using Obtainer
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
* Add helper method to URL class to check for default (index) files.

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

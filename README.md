### Setup
* Set $conf['migration_tools_base_dir'] correctly for your local machine
* Rsync down private://wordpress

### Copying unmanaged images:
To recursively copy image files from the current directory to a destination:

There is a drush command that will move the images to public:// from
$conf['migration_tools_base_dir'] for any given organization:

drush dmi <organization_abbreviation>

````
cd path/to/source/dir
find -E . -iregex '.*\.(jpg|png|gif)' | cpio --pass-through \
 --preserve-modification-time \
 --make-directories --verbose path/to/dest/dir
````

### Class reference
The following is a list of classes that exemplify various types of migrations.

* Static HTML file migration:
    * CareersJobMigration
* Chunk Parsing:
    * JusticeChunkParser
    * BriefChunkParser
    * GalleryChunkParser
* Field Collections:
    * GalleryImagesMigration
* CSV Migration:
    * OrganizationMigration
* Wordpress Migration
    * OipFoiaPostMigration
* MySQL Migration:
    * OlcOpinionMigration
* Image and File field migrations:
    * OlcOpinionMigration
    * GalleryImagesMigration
    * AgHistoricalBioMigration
* Location migration:
    * OpaSpeechMigration
* PDF parsing:
    * OsgBriefMigration
* Non-english node migration:
    * EspanolPageMigration

### Migration Generation

Since we have so much infrastructure developed around our html to node 
migrations, it made sense to automate some of the process. The 
`doj-generate-migration-class` drush command will automatically generate a 
migration class for you. See `drush doj-generate-migration-class --help` for 
command specification.

The command accepts the following arguments:
 * config: the name of a yml file containing organization specific config. These
   must be stored in scripts/districts for district and scripts/organizations
   for non-districts. See [Specific Configuration](#specific-config) for more
   information.
   
The command accepts the following options:
  * type: (optional) Indicates whether the migration class is for a district or
    a normal organization. This dictate which twig template will be used to
    generate the migration. Defaults to 'organization'. Valid values include:
    * district
    * organization

Example commands:
> drush dgm atr.yml
> drush dgm md.yml --type=district

#### Migration Types

There are two types of migrations, organization and district. Each of these
has its own global-level configuration file, which specifies the correct
twig template to use.
  * config/organization.yml
  * config/district.yml

The global configuration file can have the following keys:
  * twig: The name of the twig file inside scripts/templates that should 
    generate the migration
  * required_keys: An array ([]) of extra keys that are required by the twig 
    template.
    * There are keys that are already required, so required_keys only need to 
      have any __extra__ keys required by the template.
  * drush_local_alias: The drush alias used for your local site.

#### Specific configuration

The specific configuration file have the information required by the script to 
generate a migration. We store district specific config in scripts/district, 
and organization specific configuration in scripts/organization.

The following keys are required for all migrations:
* abbreviation: the migration abbreviation (ex. usao-ma)
* full_name: The full name of the migration (ex. District of Massachusetts)
* directory: The directory inside of migration sources that contain the 
  migration files (ex. usao/ma)

The district migrations have an extra optional key for when press releases are 
part of the migration:

* pr_subdirectory: A directory inside of the migration directory, that contain 
the press releases (ex.news)

This command will generate the migration for the District of Massachusetts. 

### Generating and Importing Menus

There are 2 drush commands to generate, and then import a menu.

The generating command looks like this:
> drush doj-generate-menu usao-az --css-selector='#navbar' --local-base-uri='usao-az' --menu-location-uri='usao/az'

The only parameter required is the abbreviation of the migration, in this 
case usao-az. 

Other configuration is optional:

* css-selector should be a css selector pointing to the outer-most ul of the 
menu in justice.gov
* local-base-uri should be the path to where the content is locally after a 
migration has been run (ex. ag or usao-nm)
* menu-location-uri should be a page in justice.gov containing the menu that 
we want to generate (ex. oarm would be saying that the menu we care about is 
locate at justice.gov/oarm)

After a migration is run, so the content is present locally (This is a 
prerequisite of menu generation), and the menu is generated, we can then 
import it.

the import command is
> drush doj-import-group-menu usao-ndny-menu.txt usao-ndny                                                              

The command takes the file where the menu is (the script assumes this file is 
inside the sources directory) and the abbreviation of the migration.

We have found 2 menu patterns in justice.gov (one mainly for orgs, and one for 
districts). 

The process differences required by the different menus is encapsulated in the 
MenuGeneratorEngine classes.

If you look at migration_tools.drush.inc you will see that the menu generation 
engine is currently hardcoded to use the district menu generation class.

A possible improvement to the code would be to allow classes to be switched 
with an option, but for now, simply changing the class there for the 
MenuGeneratorEngineDefault class will work for organization menus.


### Debugging and Iterations
There are two settings through drupal variables that can aid in building and
debugging a migration.  The default for each is FALSE but can be overidden in
your settings.local.php

variable: migration_tools_drush_debug
  Enables output to be seen in the terminal on a file by file basis to see
  what elements are being found by the obtainers and migrated. Default is FALSE.
  Enable debug output: drush vset migration_tools_drush_debug TRUE

migration_tools_drush_stop_on_error
  When migration_tools_drush_debug is TRUE and a warning is thrown by the
  messsaging system that is of the level WATCHDOG_ERROR, WATCHDOG_CRITICAL,
  WATCHDOG_ALERT, WATCHDOG_EMERGENCY.
  Default is FALSE.
  Enable stop on error: drush vset migration_tools_drush_stop_on_error TRUE

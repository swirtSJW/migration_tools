### Setup
* Set $conf['doj_migration_base_dir'] correctly for your local machine
* Rsync down private://wordpress

### Copying unmanaged images:
To recursively copy image files from the current directory to a destination:

There is a drush command that will move the images to public:// from 
$conf['doj_migration_base_dir'] for any given organization:

drush dmi <organization_abbreviation>

````
cd path/to/source/dir
find -E . -iregex '.*\.(jpg|png|gif)' | cpio --pass-through \
 --preserve-modification-time \
 --make-directories --verbose path/to/dest/dir
````

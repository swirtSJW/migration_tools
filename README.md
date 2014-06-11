### Setup
* Set $conf['doj_migration_base_dir'] correctly for your local machine
* Rsync down private://wordpress

### Copying unmanaged images:
To recursively copy image files from the current directory to a destination:
````
cd path/to/source/dir
find -E . -iregex '.*\.(jpg|png|gif)' | cpio --pass-through \
 --preserve-modification-time \
 --make-directories --verbose path/to/dest/dir
````

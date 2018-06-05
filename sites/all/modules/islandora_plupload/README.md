CONTENTS OF THIS FILE
---------------------

 * summary
 * requirements
 * installation
 * configuration
 * troubleshooting

SUMMARY
-------

Islandora Plupload integrates the Plupload library with Islandora file fields to allow for the upload of files greater than limits imposed by PHP.

REQUIREMENTS
------------

Islandora

Plupload

INSTALLATION
------------

Install Plupload (http://www.drupal.org/project/plupload) and the associated
library according to directions provided by the Plupload module except make
sure to use a 1.x version of the library as 2.x is now the default download
which does not work with the module without changes. Enable Islandora Plupload
and all managed_file form fields within forms with an ID containing "islandora"
will be replaced by the Plupload widget.

CONFIGURATION
-------------

Visit /admin/islandora/tools/plupload to set the maximum file size for Islandora Plupload.
The chunk size may also be set, this should be smaller than PHP's post_max_size.

TROUBLESHOOTING
---------------

Handling of large files for derivative generation may require boosting PHP's memory_limit and max_execution_time.

If you wish to avoid having this module alter a `managed_file` field, you may put the `#islandora_plupload_do_not_alter` flag on the given element (with a value which evaluates to `TRUE`).

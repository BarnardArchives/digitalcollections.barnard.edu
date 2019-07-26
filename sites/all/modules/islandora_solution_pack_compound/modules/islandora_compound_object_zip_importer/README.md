# Islandora Compound Object ZIP Importer

## Introduction

GUI tool for batch-ingesting children directly into a Compound CModel object.

## Requirements

This module requires the following modules/libraries:

* [Islandora](https://github.com/islandora/islandora)
* [Islandora Basic Collection](https://github.com/Islandora/islandora_solution_pack_collection)
* [Islandora ZIP Importer](https://github.com/islandora/islandora_importer/tree/7.x/modules/zip_importer)

## Installation

Install as usual, see [this](https://drupal.org/documentation/install/modules-themes/modules-7) for further information.


## Usage

To ingest children, go to the __Manage » Compound__ tab on an Islandora Compound CModel object. Select "Import objects". Upload a zip file formatted according to the Batch Ingest instructions - containing binaries and/or XML metadata, with the filenames used to match XML with their associated binaries. Select a content model to be applied to the child objects, as well as a namespace. Ingest immediately or save it to an Islandora batch set to ingest later.

Notes on expected behaviours:

* Regardless of whether the Compound Solution Pack is configured to allow all objects to have children, this tool only applies to Compound CModel objects.
* All children will have the same content model applied. If you are uploading multiple types of children, this will need to be done in different batches.
* Whether or not you upload immediately, this creates an Islandora Batch Set which is available at /islandora_batch/reports/set (requires Views).
* Children uploaded this way will not be members of other collections. If you use breadcrumbs, there is an option in the Compound Solution Pack to allow compound "parents" to show in breadcrumbs.
* The children will be ingested (and therefore ordered) alphabetically. To set the order before ingest, name the files in the batch so that they sort in the desired order (e.g. 001-myobject.tiff, 002-second-object.tiff, etc.)
* The batch ingest process does not set the thumbnail of the parent from the first child, even if that option is enabled in the Compound Solution pack. After batch importing, visit the parent's __Manage » Compound__ tab and click "Save" to generate the parent's thumbnail.


## Troubleshooting/Issues

Having problems or solved a problem? Check out the Islandora google groups for a solution.

* [Islandora Group](https://groups.google.com/forum/?hl=en&fromgroups#!forum/islandora)
* [Islandora Dev Group](https://groups.google.com/forum/?hl=en&fromgroups#!forum/islandora-dev)

## Maintainers/Sponsors

Current maintainers:

* [Diego Pino](https://github.com/diegopino)

## Development

If you would like to contribute to this module, please check out [CONTRIBUTING.md](CONTRIBUTING.md). In addition, we have helpful [Documentation for Developers](https://github.com/Islandora/islandora/wiki#wiki-documentation-for-developers) info, as well as our [Developers](http://islandora.ca/developers) section on the [Islandora.ca](http://islandora.ca) site.


## License

[GPLv3](http://www.gnu.org/licenses/gpl-3.0.txt)

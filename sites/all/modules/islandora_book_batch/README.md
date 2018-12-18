# Islandora Book Batch [![Build Status](https://travis-ci.org/Islandora/islandora_book_batch.png?branch=7.x)](https://travis-ci.org/Islandora/islandora_book_batch)

## Introduction

This module implements a batch framework for importing books into Islandora.

The ingest is a two-step process:

* Preprocessing: The data is scanned and a number of entries are created in the
  Drupal database.  There is minimal processing done at this point, so preprocessing can
  be completed outside of a batch process.
* Ingest: The data is actually processed and ingested. This happens inside of
  a Drupal batch.

## Requirements

This module requires the following modules/libraries:

* [Islandora](https://github.com/islandora/islandora)
* [Tuque](https://github.com/islandora/tuque)
* [Islandora Batch](https://github.com/Islandora/islandora_batch)
* [Book Solution Pack](https://github.com/Islandora/islandora_solution_pack_book)


## Installation

Install module as usual, see [this](https://drupal.org/documentation/install/modules-themes/modules-7) for further information.

## Configuration

N/A

## Documentation

Further documentation for this module is available at [our wiki](https://wiki.duraspace.org/display/ISLANDORA/Islandora+Book+Batch).

### Usage

The base ZIP/directory preprocessor can be called as a drush script (see `drush help islandora_book_batch_preprocess` for additional parameters):
```man
$ drush help islandora_book_batch_preprocess

Options:
 --aggregate_ocr                           A flag to cause OCR to be aggregated to books, if OCR is also being generated per-page.
 --content_models                          A comma-separated list of content models to assign to the objects. Only applies to the "book" level object.
 --create_pdfs                             A flag to cause PDFs to be created in books. Page PDF creation is dependant on the configuration within Drupal proper.
 --directory_dedup                         A flag to indicate that we should avoid repreprocessing books which are located in directories.
 --do_not_generate_hocr                    A flag to allow for conditional HOCR generation.
 --do_not_generate_ocr                     A flag to allow for conditional OCR generation.
 --email_admin                             A flag to notify the site admin when the book is fully ingested (depends on Rules being enabled).
 --namespace                               The namespace for objects created by this command.  Defaults to namespace set in fedora config.
 --output_set_id                           A flag to indicate whether to print the set ID of the preprocessed book.
 --page_progression                        A flag to indicate the page progression for the book. If not specified will default to LR.
 --parent                                  The collection to which the generated items should be added.  Only applies to the "book" level object. If "directory" and the directory
                                           containing the book description is a valid PID, it will be set as the parent. If this is specified and itself is a PID, all books will be
                                           related to the given PID.
 --parent_relationship_pred                The predicate of the relationship to the parent. Defaults to "isMemberOfCollection".
 --parent_relationship_uri                 The namespace URI of the relationship to the parent. Defaults to "info:fedora/fedora-system:def/relations-external#".
 --target                                  The target to directory or zip file to scan. Required.
 --type                                    Either "directory" or "zip". Required.
 --wait_for_metadata                       A flag to indicate that we should hold off on trying to ingest books until we have metadata available for themat the book level.

Aliases: ibbp
```

Books must be broken up into separate directories, such that each directory at the "top" level (in the target directory or Zip file) represents a book. Book pages are their own directories inside of each book directory containing an OBJ and additional datastreams can be added here manually.

Files are assigned to object datastreams based on their basename, so a folder structure like:

**Note:** A Metadata file (\*.xml **OR** \*.mrc) and a **Obj**ect file is required, all other datastreams are optional.

**Option 1**
```tree-view
/tmp/batch_ingest/
 └── book/
      ├── 1/
      │   └── OBJ.tif
      ├── 2/
      │   ├── OBJ.tif
      │   ├── OCR.asc
      │   └── HOCR.shtml
      └── DC.xml
```

**Option 2**
```tree-view
/tmp/batch_ingest/
└── book/
    ├── 1/
    │   ├── DC.xml
    │   └── OBJ.tif
    └── 2/
        ├── DC.xml
        └── OBJ.tif
```

Either would result in a two-page book.

Each page directory name will be used as the sequence number of the page created.

A file named --METADATA--.xml can contain either MODS, DC or MARCXML which is used to fill in the MODS or DC streams (if not provided explicitly). Similarly, --METADATA--.mrc (containing binary MARC) will be transformed to MODS and then possibly to DC, if neither are provided explicitly.

If no MODS is provided at the book level - either directly as MODS.xml, or transformed from either a DC.xml or the "--METADATA--" file discussed above - the directory name will be used as the title.

Drush made the `target` parameter reserved as of Drush 7. To allow for backwards compatability this will be preserved.

Drush 7 and above: (Examples of **Zip** and **Directory** batch preprocessing)

`drush -v -u 1 --uri=http://localhost islandora_book_batch_preprocess --type=zip --scan_target=/path/to/archive.zip`

`drush -v -u 1 --uri=http://localhost islandora_book_batch_preprocess --namespace=book --type=directory --scan_target=/tmp/batch_ingest/`

Drush 6 and below: (Examples of **Zip** and **Directory** batch preprocessing)

`drush -v -u 1 --uri=http://localhost islandora_book_batch_preprocess --type=zip --target=/path/to/archive.zip`

`drush -v -u 1 --uri=http://localhost islandora_book_batch_preprocess --namespace=book --type=directory --target=/tmp/batch_ingest/`

This will populate the queue (stored in the Drupal database) with base entries.

The queue of preprocessed items can then be processed:

`drush -v --user=admin --uri=http://localhost islandora_batch_ingest`

### Customization

Custom ingests can be written by [extending](https://github.com/Islandora/islandora_batch/wiki/How-To-Extend) any of the existing preprocessors and batch object implementations.

## Troubleshooting/Issues

Having problems or solved a problem? Check out the Islandora google groups for a solution.

* [Islandora Group](https://groups.google.com/forum/?hl=en&fromgroups#!forum/islandora)
* [Islandora Dev Group](https://groups.google.com/forum/?hl=en&fromgroups#!forum/islandora-dev)

## Maintainers/Sponsors

Current maintainers:

* [Adam Vessey](https://github.com/adam-vessey)

## Development

If you would like to contribute to this module, please check out [CONTRIBUTING.md](CONTRIBUTING.md). In addition, we have helpful [Documentation for Developers](https://github.com/Islandora/islandora/wiki#wiki-documentation-for-developers) info, as well as our [Developers](http://islandora.ca/developers) section on the [Islandora.ca](http://islandora.ca) site.

## License

[GPLv3](http://www.gnu.org/licenses/gpl-3.0.txt)

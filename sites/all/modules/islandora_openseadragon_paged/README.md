# Islandora OpenSeadragon Paged

## Introduction

This module provides OpenSeadragon as a viewer for paged Islandora content (specificially, newspaper issues, books, and manuscripts).

## Requirements

This module requires the following modules/libraries:

* [Islandora](https://github.com/islandora/islandora)
* [Tuque](https://github.com/islandora/tuque)
* [Islandora OpenSeadragon](https://github.com/Islandora/islandora_openseadragon)

## Installation

Install as usual, see [this](https://drupal.org/documentation/install/modules-themes/modules-7) for further information.

## Configuration

Set configuration options at Administration » Islandora » Islandora Viewers » OpenSeadragon paged (admin/islandora/islandora_viewers/openseadragon_paged).

You will also need to implement a template corresponding to the type of object you hope to display in the viewer on the theme layer, since the book and newspaper modules wrap the viewer in `<div id="book-viewer">`.

## Troubleshooting/Issues

Having problems or solved a problem? Check out the Islandora google groups for a solution.

* [Islandora Group](https://groups.google.com/forum/?hl=en&fromgroups#!forum/islandora)
* [Islandora Dev Group](https://groups.google.com/forum/?hl=en&fromgroups#!forum/islandora-dev)

## Maintainers/Sponsors

This module is currently maintained by [Dillon Savage](http://github.com/dillonsavage) and was developed for use in the [Barnard College Digital Collections](http://digitalcollections.barnard.edu). (See also: [Barnard Archives and Special Collections](http://archives.barnard.edu/), [BarnardArchives](http://github.com/BarnardArchives) on github.)

## Development

If you would like to contribute to this module, please check out [CONTRIBUTING.md](CONTRIBUTING.md). In addition, we have helpful [Documentation for Developers](https://github.com/Islandora/islandora/wiki#wiki-documentation-for-developers) info, as well as our [Developers](http://islandora.ca/developers) section on the [Islandora.ca](http://islandora.ca) site.

## License

[GPLv3](http://www.gnu.org/licenses/gpl-3.0.txt)

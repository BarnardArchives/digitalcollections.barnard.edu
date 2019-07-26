# Compound Object Solution Pack [![Build Status](https://travis-ci.org/Islandora/islandora_solution_pack_compound.png?branch=7.x)](https://travis-ci.org/Islandora/islandora_solution_pack_compound)

## Introduction

The Islandora Compound Object Solution Pack enables the creation and management of parent-child relationships between objects, and an interface to navigate between children of the same object. Children have an order within their parent, which can be managed from the parent object.

## Requirements

This module requires the following modules/libraries:

* [Islandora](https://github.com/islandora/islandora)

Specific features require the following modules/libraries:

* [Islandora Solr Search](https://github.com/Islandora/islandora_solr_search) Module
    * Provides a Solr backend to retrieve children instead of using the resource index
    * Configurable ability to hide child objects from Solr search results

* [JAIL](https://github.com/sebarmeli/JAIL) JQuery library 
    * For the JAIL Display (lazy-loading) block


## Installation

Install as usual, see [this](https://drupal.org/documentation/install/modules-themes/modules-7) for further information.

If using the JAIL display, the [JAIL](https://github.com/sebarmeli/JAIL)
library must be present within sites/all/libraries/JAIL.

## Usage

This module provides a "Compound CModel". Objects of this type are shells to hold children. They have no content of their own, and the object page at a Compound CModel object displays the content and metadata of its first child. This module can be configured to allow other objects to have children, in which case, the parent object appears as usual, and the navigation block displays the parent followed by its children.

Compound relationships are managed through the __Manage » Compound__ tab which appears on all objects.

Navigation between objects linked by a Compound relationship requires a block to be placed on the interface in __Structure » Blocks__. This module provides two options: a standard Islandora Compound Object Navigation block, and the Islandora Compound JAIL Display, which uses a javascript library for lazy-loading (improving performance on compound objects with many children).

![compobjblocks_to_configure](docs/compound-blocks.png)


## Configuration

Options for this module can be set at 
__Administration » Islandora » Solution pack configuration » Compound Object Solution Pack__  (`admin/islandora/solution_pack_config/compound_object`). Configuration options are documented further in [our Wiki](https://wiki.duraspace.org/display/ISLANDORA/Compound+Solution+Pack).

__New in 7.x-1.12:__ If the Solr Search Module is enabled, you can use Solr instead of SPARQL to query for Compound membership. Options for this can be configured on the __Solr backend__ tab (`admin/islandora/solution_pack_config/compound_object/solr`).


![Configuration](https://user-images.githubusercontent.com/25011926/39889778-d1a91aca-5466-11e8-8eb1-1978cac81104.png)


**Theme:**

The "Islandora Compound Object Navigation" block can be themed. See `theme_islandora_compound_prev_next()`.

**Batch Ingest:**

A zip importer for child objects is available as a submodule, and documentation is in its own README.

**Drush:**

If compound objects were created before 7.x-1.2, they will use the relationship `isPartOf` instead of `isConstituentOf`. A drush command, `drush update_rels_predicate`, can be run from the command line to update these predicates. To use it, temporarily set the __Child relationship predicate__ in the compound solution pack to `isPartOf`. The script will set it to `isConstituentOf` when it finishes.

Notes on usage:
* The script acts on the children of Compound CModel objects; it does not affect compounds where the parent is a different type.
* If anonymous cannot view Islandora objects, then the drush script must be explicitly run as a user who can.

## Documentation

Further documentation for this module is available at [our wiki](https://wiki.duraspace.org/display/ISLANDORA/Compound+Solution+Pack).

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

# Barnard College Islandora

Custom Drupal functionality for [Barnard Digital Collections](http://digitalcollections.barnard.edu).

## Dependencies

Islandora modules:

* [Islandora](https://github.com/Islandora/islandora)
* [Large Image Solution Pack](https://github.com/Islandora/islandora_solution_pack_large_image)
* [Newspaper Solution Pack](https://github.com/Islandora/islandora_solution_pack_newspaper)
* [Islandora OpenSeadragon](https://github.com/Islandora/islandora_openseadragon)
* [Islandora Paged Content](https://github.com/Islandora/islandora_paged_content)

Some of `bc_islandora`'s custom theme functions are invoked by the Digital Collections site's theme, [barnard_theme](https://github.com/BarnardArchives/barnard_theme), instead of by the module itself. The content types and taxonomy terms whose existence the module assumes can be created via [Features](http://drupal.org/project/features) and [dc_content_types](https://github.com/BarnardArchives/digitalcollections.barnard.edu/tree/master/sites/all/modules/dc_content_types).

The [Sass preprocessor](http://sass-lang.com/) and [Compass](http://compass-style.org/) are used to generate CSS.

## Newspapers

We display newspapers using the [islandora_openseadragon_paged](https://github.com/BarnardArchives/islandora_openseadragon_paged) viewer.

## Breadcrumbs

A theme function called `theme_bc_islandora_breadcrumb()`, which is invoked on the theme layer, provides custom breadcrumb behavior according to the following logic: if the object's MODS identifier corresponds with the book, newspaper, or student publication collections, links to the appropriate landing page, and to the collections landing page, are provided. Otherwise, links to canned solr searches are formed using the object's `relatedItem` MODS field.

## Featured object

The function `_bc_islandora_featured()` supplies the image path and caption of a randomly-selected `featured_object` node to the theme layer via a JavaScript setting called `featured_img_path`. Its return value is the value of the caption, which, if the featured node includes a pid, is linked to the corresponding object page.

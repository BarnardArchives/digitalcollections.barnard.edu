<?php
/**
 * @file
 * barnard_theme_feature.features.menu_links.inc
 */

/**
 * Implements hook_menu_default_menu_links().
 */
function barnard_theme_feature_menu_default_menu_links() {
  $menu_links = array();

  // Exported menu link: main-menu_about:node/1
  $menu_links['main-menu_about:node/1'] = array(
    'menu_name' => 'main-menu',
    'link_path' => 'node/1',
    'router_path' => 'node/%',
    'link_title' => 'About',
    'options' => array(
      'identifier' => 'main-menu_about:node/1',
    ),
    'module' => 'menu',
    'hidden' => 0,
    'external' => 0,
    'has_children' => 0,
    'expanded' => 0,
    'weight' => -46,
    'customized' => 1,
  );
  // Exported menu link: main-menu_about:node/2
  $menu_links['main-menu_about:node/2'] = array(
    'menu_name' => 'main-menu',
    'link_path' => 'node/2',
    'router_path' => 'node/%',
    'link_title' => 'About',
    'options' => array(
      'identifier' => 'main-menu_about:node/2',
    ),
    'module' => 'menu',
    'hidden' => 0,
    'external' => 0,
    'has_children' => 0,
    'expanded' => 0,
    'weight' => -46,
    'customized' => 0,
  );
  // Exported menu link: main-menu_all-collections:islandora
  $menu_links['main-menu_all-collections:islandora'] = array(
    'menu_name' => 'main-menu',
    'link_path' => 'islandora',
    'router_path' => 'islandora',
    'link_title' => 'All collections',
    'options' => array(
      'attributes' => array(),
      'identifier' => 'main-menu_all-collections:islandora',
    ),
    'module' => 'menu',
    'hidden' => 0,
    'external' => 0,
    'has_children' => 0,
    'expanded' => 0,
    'weight' => -48,
    'customized' => 1,
  );
  // Exported menu link: main-menu_browse:islandora/search/
  $menu_links['main-menu_browse:islandora/search/'] = array(
    'menu_name' => 'main-menu',
    'link_path' => 'islandora/search/',
    'router_path' => 'islandora/search',
    'link_title' => 'Browse',
    'options' => array(
      'query' => array(
        'type' => 'dismax',
      ),
      'attributes' => array(),
      'identifier' => 'main-menu_browse:islandora/search/',
    ),
    'module' => 'menu',
    'hidden' => 0,
    'external' => 0,
    'has_children' => 0,
    'expanded' => 0,
    'weight' => -49,
    'customized' => 1,
  );
  // Exported menu link: main-menu_exhibits:islandora
  $menu_links['main-menu_exhibits:islandora'] = array(
    'menu_name' => 'main-menu',
    'link_path' => 'islandora',
    'router_path' => 'islandora',
    'link_title' => 'Exhibits',
    'options' => array(
      'attributes' => array(),
      'identifier' => 'main-menu_exhibits:islandora',
    ),
    'module' => 'menu',
    'hidden' => 0,
    'external' => 0,
    'has_children' => 0,
    'expanded' => 0,
    'weight' => -47,
    'customized' => 1,
  );
  // Exported menu link: main-menu_home:<front>
  $menu_links['main-menu_home:<front>'] = array(
    'menu_name' => 'main-menu',
    'link_path' => '<front>',
    'router_path' => '',
    'link_title' => 'Home',
    'options' => array(
      'identifier' => 'main-menu_home:<front>',
    ),
    'module' => 'menu',
    'hidden' => 1,
    'external' => 1,
    'has_children' => 0,
    'expanded' => 0,
    'weight' => -50,
    'customized' => 1,
  );
  // Exported menu link: menu-collections_exhibits:islandora/object
  $menu_links['menu-collections_exhibits:islandora/object'] = array(
    'menu_name' => 'menu-collections',
    'link_path' => 'islandora/object',
    'router_path' => 'islandora',
    'link_title' => 'Exhibits',
    'options' => array(
      'attributes' => array(),
      'identifier' => 'menu-collections_exhibits:islandora/object',
    ),
    'module' => 'menu',
    'hidden' => 0,
    'external' => 0,
    'has_children' => 0,
    'expanded' => 0,
    'weight' => -48,
    'customized' => 1,
  );
  // Exported menu link: menu-collections_newspaper:islandora/object
  $menu_links['menu-collections_newspaper:islandora/object'] = array(
    'menu_name' => 'menu-collections',
    'link_path' => 'islandora/object',
    'router_path' => 'islandora',
    'link_title' => 'Newspaper',
    'options' => array(
      'attributes' => array(),
      'identifier' => 'menu-collections_newspaper:islandora/object',
    ),
    'module' => 'menu',
    'hidden' => 0,
    'external' => 0,
    'has_children' => 0,
    'expanded' => 0,
    'weight' => -50,
    'customized' => 1,
  );
  // Exported menu link: menu-collections_photographs:islandora/object/islandora:sp_basic_image_collection
  $menu_links['menu-collections_photographs:islandora/object/islandora:sp_basic_image_collection'] = array(
    'menu_name' => 'menu-collections',
    'link_path' => 'islandora/object/islandora:sp_basic_image_collection',
    'router_path' => 'islandora/object/%',
    'link_title' => 'Photographs',
    'options' => array(
      'attributes' => array(),
      'identifier' => 'menu-collections_photographs:islandora/object/islandora:sp_basic_image_collection',
    ),
    'module' => 'menu',
    'hidden' => 0,
    'external' => 0,
    'has_children' => 0,
    'expanded' => 0,
    'weight' => -47,
    'customized' => 1,
  );
  // Exported menu link: menu-collections_yearbook:islandora/object
  $menu_links['menu-collections_yearbook:islandora/object'] = array(
    'menu_name' => 'menu-collections',
    'link_path' => 'islandora/object',
    'router_path' => 'islandora',
    'link_title' => 'Yearbook',
    'options' => array(
      'attributes' => array(),
      'identifier' => 'menu-collections_yearbook:islandora/object',
    ),
    'module' => 'menu',
    'hidden' => 0,
    'external' => 0,
    'has_children' => 0,
    'expanded' => 0,
    'weight' => -49,
    'customized' => 1,
  );
  // Exported menu link: menu-frontpage-menu_browse-all-materials:islandora/search/
  $menu_links['menu-frontpage-menu_browse-all-materials:islandora/search/'] = array(
    'menu_name' => 'menu-frontpage-menu',
    'link_path' => 'islandora/search/',
    'router_path' => 'islandora/search',
    'link_title' => 'Browse all materials',
    'options' => array(
      'attributes' => array(),
      'query' => array(
        'type' => 'dismax',
      ),
      'identifier' => 'menu-frontpage-menu_browse-all-materials:islandora/search/',
    ),
    'module' => 'menu',
    'hidden' => 0,
    'external' => 0,
    'has_children' => 0,
    'expanded' => 0,
    'weight' => -50,
    'customized' => 1,
  );
  // Exported menu link: menu-frontpage-menu_browse-collections:islandora
  $menu_links['menu-frontpage-menu_browse-collections:islandora'] = array(
    'menu_name' => 'menu-frontpage-menu',
    'link_path' => 'islandora',
    'router_path' => 'islandora',
    'link_title' => 'Browse collections',
    'options' => array(
      'attributes' => array(),
      'identifier' => 'menu-frontpage-menu_browse-collections:islandora',
    ),
    'module' => 'menu',
    'hidden' => 0,
    'external' => 0,
    'has_children' => 0,
    'expanded' => 0,
    'weight' => -49,
    'customized' => 1,
  );
  // Exported menu link: menu-frontpage-menu_newspaper:islandora/object
  $menu_links['menu-frontpage-menu_newspaper:islandora/object'] = array(
    'menu_name' => 'menu-frontpage-menu',
    'link_path' => 'islandora/object',
    'router_path' => 'islandora',
    'link_title' => 'Newspaper',
    'options' => array(
      'attributes' => array(),
      'identifier' => 'menu-frontpage-menu_newspaper:islandora/object',
    ),
    'module' => 'menu',
    'hidden' => 0,
    'external' => 0,
    'has_children' => 0,
    'expanded' => 0,
    'weight' => -48,
    'customized' => 1,
  );
  // Exported menu link: menu-frontpage-menu_photographs:islandora/object
  $menu_links['menu-frontpage-menu_photographs:islandora/object'] = array(
    'menu_name' => 'menu-frontpage-menu',
    'link_path' => 'islandora/object',
    'router_path' => 'islandora',
    'link_title' => 'Photographs',
    'options' => array(
      'attributes' => array(),
      'identifier' => 'menu-frontpage-menu_photographs:islandora/object',
    ),
    'module' => 'menu',
    'hidden' => 0,
    'external' => 0,
    'has_children' => 0,
    'expanded' => 0,
    'weight' => -46,
    'customized' => 1,
  );
  // Exported menu link: menu-frontpage-menu_yearbook:islandora/object
  $menu_links['menu-frontpage-menu_yearbook:islandora/object'] = array(
    'menu_name' => 'menu-frontpage-menu',
    'link_path' => 'islandora/object',
    'router_path' => 'islandora',
    'link_title' => 'Yearbook',
    'options' => array(
      'attributes' => array(),
      'identifier' => 'menu-frontpage-menu_yearbook:islandora/object',
    ),
    'module' => 'menu',
    'hidden' => 0,
    'external' => 0,
    'has_children' => 0,
    'expanded' => 0,
    'weight' => -47,
    'customized' => 1,
  );
  // Translatables
  // Included for use with string extractors like potx.
  t('About');
  t('All collections');
  t('Browse');
  t('Browse all materials');
  t('Browse collections');
  t('Exhibits');
  t('Home');
  t('Newspaper');
  t('Photographs');
  t('Yearbook');


  return $menu_links;
}

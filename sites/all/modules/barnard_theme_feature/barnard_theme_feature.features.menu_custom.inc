<?php
/**
 * @file
 * barnard_theme_feature.features.menu_custom.inc
 */

/**
 * Implements hook_menu_default_menu_custom().
 */
function barnard_theme_feature_menu_default_menu_custom() {
  $menus = array();

  // Exported menu: main-menu.
  $menus['main-menu'] = array(
    'menu_name' => 'main-menu',
    'title' => 'Main menu',
    'description' => 'The <em>Main</em> menu is used on many sites to show the major sections of the site, often in a top navigation bar.',
  );
  // Exported menu: menu-collections.
  $menus['menu-collections'] = array(
    'menu_name' => 'menu-collections',
    'title' => 'Collections',
    'description' => 'Static links to top level collection pages',
  );
  // Exported menu: menu-frontpage-menu.
  $menus['menu-frontpage-menu'] = array(
    'menu_name' => 'menu-frontpage-menu',
    'title' => 'Frontpage Menu',
    'description' => '',
  );
  // Translatables
  // Included for use with string extractors like potx.
  t('Collections');
  t('Frontpage Menu');
  t('Main menu');
  t('Static links to top level collection pages');
  t('The <em>Main</em> menu is used on many sites to show the major sections of the site, often in a top navigation bar.');


  return $menus;
}

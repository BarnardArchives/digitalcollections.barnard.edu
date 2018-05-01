<?php
/**
 * @file
 * barnard_theme_feature.features.fe_block_boxes.inc
 */

/**
 * Implements hook_default_fe_block_boxes().
 */
function barnard_theme_feature_default_fe_block_boxes() {
  $export = array();

  $fe_block_boxes = new stdClass();
  $fe_block_boxes->info = 'Footer Content';
  $fe_block_boxes->format = 'full_html';
  $fe_block_boxes->machine_name = 'footer_content';
  $fe_block_boxes->body = '<a href="/home">Barnard Archives And Special Collections</a>';

  $export['footer_content'] = $fe_block_boxes;

  $fe_block_boxes = new stdClass();
  $fe_block_boxes->info = 'Frontpage Navigation Panel';
  $fe_block_boxes->format = 'php_code';
  $fe_block_boxes->machine_name = 'frontpage_panel';
  $fe_block_boxes->body = '<h1>Barnard Digital Collections</h1>
<p>A little text here see more <a href="about">about</a> the collections etc</p>
<?php
$form = drupal_get_form(\'islandora_solr_simple_search_form\');
print drupal_render($form);
//
$menu = menu_navigation_links(\'menu-frontpage-menu\');
print theme(\'links__menu_frontpage_menu\', array(\'links\' => $menu));
?>';

  $export['frontpage_panel'] = $fe_block_boxes;

  return $export;
}

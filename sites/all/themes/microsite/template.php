<?php
// $Id: template.php,v 1.21 2009/08/12 04:25:15 johnalbin Exp $

/**
 * @file
 * Contains theme override functions and preprocess functions for the theme.
 *
 * ABOUT THE TEMPLATE.PHP FILE
 *
 *   The template.php file is one of the most useful files when creating or
 *   modifying Drupal themes. You can add new regions for block content, modify
 *   or override Drupal's theme functions, intercept or make additional
 *   variables available to your theme, and create custom PHP logic. For more
 *   information, please visit the Theme Developer's Guide on Drupal.org:
 *   http://drupal.org/theme-guide
 *
 * OVERRIDING THEME FUNCTIONS
 *
 *   The Drupal theme system uses special theme functions to generate HTML
 *   output automatically. Often we wish to customize this HTML output. To do
 *   this, we have to override the theme function. You have to first find the
 *   theme function that generates the output, and then "catch" it and modify it
 *   here. The easiest way to do it is to copy the original function in its
 *   entirety and paste it here, changing the prefix from theme_ to microsite_.
 *   For example:
 *
 *     original: theme_breadcrumb()
 *     theme override: microsite_breadcrumb()
 *
 *   where microsite is the name of your sub-theme. For example, the
 *   zen_classic theme would define a zen_classic_breadcrumb() function.
 *
 *   If you would like to override any of the theme functions used in Zen core,
 *   you should first look at how Zen core implements those functions:
 *     theme_breadcrumbs()      in zen/template.php
 *     theme_menu_item_link()   in zen/template.php
 *     theme_menu_local_tasks() in zen/template.php
 *
 *   For more information, please visit the Theme Developer's Guide on
 *   Drupal.org: http://drupal.org/node/173880
 *
 * CREATE OR MODIFY VARIABLES FOR YOUR THEME
 *
 *   Each tpl.php template file has several variables which hold various pieces
 *   of content. You can modify those variables (or add new ones) before they
 *   are used in the template files by using preprocess functions.
 *
 *   This makes THEME_preprocess_HOOK() functions the most powerful functions
 *   available to themers.
 *
 *   It works by having one preprocess function for each template file or its
 *   derivatives (called template suggestions). For example:
 *     THEME_preprocess_page    alters the variables for page.tpl.php
 *     THEME_preprocess_node    alters the variables for node.tpl.php or
 *                              for node-forum.tpl.php
 *     THEME_preprocess_comment alters the variables for comment.tpl.php
 *     THEME_preprocess_block   alters the variables for block.tpl.php
 *
 *   For more information on preprocess functions and template suggestions,
 *   please visit the Theme Developer's Guide on Drupal.org:
 *   http://drupal.org/node/223440
 *   and http://drupal.org/node/190815#template-suggestions
 */


/**
 * Implementation of HOOK_theme().
 */
function microsite_theme(&$existing, $type, $theme, $path) {
  $hooks = zen_theme($existing, $type, $theme, $path);
  // Add your theme hooks like this:
  /*
  $hooks['hook_name_here'] = array( // Details go here );
  */
  // @TODO: Needs detailed comments. Patches welcome!
  return $hooks;
}

/**
 * Override or insert variables into all templates.
 *
 * @param $vars
 *   An array of variables to pass to the theme template.
 * @param $hook
 *   The name of the template being rendered (name of the .tpl.php file.)
 */
/* -- Delete this line if you want to use this function
function microsite_preprocess(&$vars, $hook) {
  $vars['sample_variable'] = t('Lorem ipsum.');
}
// */

/**
 * Override or insert variables into the page templates.
 *
 * @param $vars
 *   An array of variables to pass to the theme template.
 * @param $hook
 *   The name of the template being rendered ("page" in this case.)
 */
function microsite_preprocess_page(&$vars, $hook) {
  global $base_url;
  //TODO: Figure out if this is the right way to build this path
  $vars['site_identity'] = $base_url . '/' . path_to_theme() . '/barnard-site-identity.png';
  // Get the primary domain path correctly.
  //$default = domain_load(0);
  //$vars['site_main_url'] = $default['path'];
  $vars['site_main_url'] = '/';
  
  $vars['date'] = '';
  
  if (isset($vars['node']) && $node = $vars['node']) {
    if (!in_array($node->type, array('slideshow', 'article', 'microsite_landing_page', 'article_wide'))) {
      $vars['date'] = format_date($vars['node']->created, 'custom', 'F d, Y');
    }
  
    if ($node->type == 'microsite_landing_page') {
      // Need the instance of field_landingpagemessage for the text_sanitize function
      $field_instance = field_info_instance('node', 'field_landingpagemessage', 'node');
      // Try to get the field items for the field_landingpagemessage.
      if ($field_items = field_get_items('node', $node, 'field_landingpagemessage')) {
        // Grab the sanitized value of the landing page message
        $landing_page_message = _text_sanitize($field_instance, LANGUAGE_NONE, $field_items[0], 'value');
      }
    
      // Reset the page title. The title in <head> is kept to the original title.
      if ($landing_page_message) {
        $vars['title'] = $landing_page_message;
      }
      else {
        $vars['title'] = '';
      }
    }
  }
}

/*
 * Remove the sidebar_first region if the node type is audience portal.
 * Remove the sidebar_secon region if the node type is an article_wide.
 */
function microsite_page_alter(&$page) {
  // Grab the key of nodes array to find out what the node id is.
  if(!empty($page['content']['system_main']['nodes'])) {
    $nid = key($page['content']['system_main']['nodes']);
  }
  
  // Remove the sidebar_first from the page array.
  if(!empty($nid) && $page['content']['system_main']['nodes'][$nid]['#node']->type == 'audience_portal') {
    unset($page['sidebar_first']);
  }
  
  // Remove the sidebar_second from the page array.
  if(!empty($nid) && $page['content']['system_main']['nodes'][$nid]['#node']->type == 'aricle_wide_format') {
    unset($page['sidebar_second']);
  }
}

/**
 * Override or insert variables into the node templates.
 *
 * @param $vars
 *   An array of variables to pass to the theme template.
 * @param $hook
 *   The name of the template being rendered ("node" in this case.)
 */
function microsite_preprocess_node(&$vars, $hook) {

  // Optionally, run node-type-specific preprocess functions, like
  // microsite_preprocess_node_page() or microsite_preprocess_node_story().
  $function = __FUNCTION__ . '_' . $vars['node']->type;
  if (function_exists($function)) {
    $function($vars, $hook);
  }
}

/**
 * Unset field_slideshow from the node content on microsite_landing_page.
 */
function microsite_preprocess_node_microsite_landing_page(&$vars, $hook) {
  unset($vars['content']['field_slideshow']);
}

function microsite_preprocess_node_slideshow(&$vars, $hook) {
  unset($vars['content']['links']['node']);
  unset($vars['content']['addthis']);
}

/**
 * Add department specific content to a department landing page
 */
function microsite_preprocess_node_department(&$vars, $hook) {
  // Create the navigation display. Active is set by current url. Yes, yucky.
  $vars['department_navigation'] = course_department_navigation_links($vars['field_department_list']['und'][0]['value'], 'department');
}

/**
 * Add department specific content to a department requirements page
 */
function microsite_preprocess_node_department_requirements(&$vars, $hook) {
  // Create the navigation display. Active is set by current url. Yes, yucky.
  $vars['department_navigation'] = course_department_navigation_links($vars['field_department_list']['und'][0]['value'], 'requirements');
}


/**
 * Override or insert variables into the comment templates.
 *
 * @param $vars
 *   An array of variables to pass to the theme template.
 * @param $hook
 *   The name of the template being rendered ("comment" in this case.)
 */
/* -- Delete this line if you want to use this function
function microsite_preprocess_comment(&$vars, $hook) {
  $vars['sample_variable'] = t('Lorem ipsum.');
}
// */

/**
 * Override or insert variables into the block templates.
 *
 * @param $vars
 *   An array of variables to pass to the theme template.
 * @param $hook
 *   The name of the template being rendered ("block" in this case.)
 */
/* -- Delete this line if you want to use this function
function microsite_preprocess_block(&$vars, $hook) {
  $vars['sample_variable'] = t('Lorem ipsum.');
}
// */
 
/**
 * The styleguide requires È added to the more text.
 * TODO: This is not affecting the more links being printed through view blocks.
 * TODO: Turn on theme_developer http://drupal.org/node/634402 once it is working to fix that function.
 */
function microsite_more_link($variables) {
  return '<div class="more-link">' . l(t('More »'), $variables['url'], array('attributes' => array('title' => $variables['title']))) . '</div>';
}

/**
 * Theme the AddThis button.
 * Addthis button does not have a class on it to differentiate it from other links
 * Adding class="addthis-link"
 */
function microsite_addthis_button($variables) {
  $build_mode = $variables['build_mode'];
  $https = isset($_SERVER['HTTPS']) && $_SERVER['HTTPS'] == 'on';
  if (variable_get('addthis_dropdown_disabled', '0')) {
    return ( sprintf('
      <a href="http://www.addthis.com/bookmark.php"
        onclick="addthis_url   = location.href; addthis_title = document.title; return addthis_click(this);" class="addthis-link">
      <img src="%s" width="%d" height="%d" %s /></a>
      ',
      $https ? addslashes(variable_get('addthis_image_secure', 'https://secure.addthis.com/button1-share.gif')) : addslashes(variable_get('addthis_image', 'http://s9.addthis.com/button1-share.gif')),
      addslashes(variable_get('addthis_image_width', '125')),
      addslashes(variable_get('addthis_image_height', '16')),
      addslashes(variable_get('addthis_image_attributes', 'alt=""'))
    ));
  }
  else {
    return ( sprintf('
      <a href="http://www.addthis.com/bookmark.php"
        onmouseover="return addthis_open(this, \'\', \'%s\', \'%s\')"
        onmouseout="addthis_close()"
        onclick="return addthis_sendto()" class="addthis-link"><img src="%s" width="%d" height="%d" %s /></a>
      <script type="text/javascript" src="%s/js/' . variable_get('addthis_widget_version', '152') . '/addthis_widget.js"></script>
      ',
      ($build_mode == 'teaser') ? url('node/'. $node->nid, array('absolute' => 1) ) : '[URL]',
      ($build_mode == 'teaser') ? addslashes($node->title) : '[TITLE]',
      $https == 'on' ? addslashes(check_plain(variable_get('addthis_image_secure', 'https://secure.addthis.com/button1-share.gif'))) : addslashes(check_plain(variable_get('addthis_image', 'http://s9.addthis.com/button1-share.gif'))),
      addslashes(variable_get('addthis_image_width', '125')),
      addslashes(variable_get('addthis_image_height', '16')),
      filter_xss(variable_get('addthis_image_attributes', 'alt=""')),
      $https == 'on' ? 'https://secure.addthis.com' : 'http://s7.addthis.com'
    ));
  }
}

/**
 * Returns HTML for a field.
 * * @param $variables
 *   An associative array containing:
 *   - label_hidden: A boolean indicating to show or hide the field label.
 *   - title_attributes: A string containing the attributes for the title.
 *   - label: The label for the field.
 *   - content_attributes: A string containing the attaributes for the content's
 *     div.
 *   - items: An array of field items.
 *   - item_attributes: An array of attributes for each item.
 *   - classes: A string containing the classes for the wrapping div.
 *   - attributes: A string containing the attributes for the wrapping div.
 *
 * @see template_preprocess_field()
 * @see template_process_field()
 * @see field.tpl.php
 *
 * @ingroup themeable
 * TODO: Figure out how to make this override for micrositesubtitle field to wrap the output with h2s.
 * TODO: or figure out how to write a theme formatter to set it to h2.
 */
function microsite_field($variables) {
  $output = '';
  
  if(!empty($variables['items'])) {

    // Render the label, if it's not hidden.
    if (!$variables['label_hidden']) {
      $output .= '<div class="field-label"' . $variables['title_attributes'] . '>' . $variables['label'] . ':&nbsp;</div>';
    }
  
    // Render the items.
    $output .= '<div class="field-items"' . $variables['content_attributes'] . '>';
    foreach ($variables['items'] as $delta => $item) {
      $classes = 'field-item ' . ($delta % 2 ? 'odd' : 'even');
      
      //Store the rendered item in a variable to check if its content is <p><br /></p> 
      $rendered_item = trim(drupal_render($item));
  
      //Check if ckeditor is inserting a br tag and return early if this is true.
      if ($rendered_item == '<p><br /></p>' || empty($rendered_item)) {
        return;
      }
      
      $output .= '<div class="' . $classes . '"' . $variables['item_attributes'][$delta] . '>' . $rendered_item . '</div>';
    }
    $output .= '</div>';
  
    // Render the top-level DIV.
    $output = '<div class="' . $variables['classes'] . ' "' . $variables['attributes'] . '>' . $output . '</div>';
  }

  return $output;
}

/*
 * Run the field through a function to output a comma separated list.
 */
function microsite_field__field_department_professors ($variables) {
  return microsite_comma_separated_list($variables);
}

/*
 * Run the field through a function to output a comma separated list.
 */
function microsite_field__field_department_assoc_prof ($variables) {
  return microsite_comma_separated_list($variables);
}

/*
 * Run the field through a function to output a comma separated list.
 */
function microsite_field__field_department_assist_prof ($variables) {
  return microsite_comma_separated_list($variables);
}

/*
 * Create a comma separated list of field items
 */
function microsite_comma_separated_list($variables) {
  $output = '';

  // Render the label, if it's not hidden.
  if (!$variables['label_hidden']) {
    $output .= '<div class="field-label"' . $variables['title_attributes'] . '>' . $variables['label'] . ':&nbsp;</div>';
  }

  // Render the items.
  $output .= '<div class="field-items"' . $variables['content_attributes'] . '>';
  foreach ($variables['items'] as $delta => $item) {
    $classes = 'field-item ' . ($delta % 2 ? 'odd' : 'even');
    if($delta != 0) {
      $output .= '<span class="' . $classes . '"' . $variables['item_attributes'][$delta] . '>, ' . drupal_render($item) . '</span>';
    } else {
      $output .= '<span class="' . $classes . '"' . $variables['item_attributes'][$delta] . '>' . drupal_render($item) . '</span>';
    }
  }
  $output .= '</div>';

  // Render the top-level DIV.
  $output = '<div class="' . $variables['classes'] . ' "' . $variables['attributes'] . '>' . $output . '</div>';

  return $output;
}

/**
 * Change the info on the search results to only display publish date.
 */
function microsite_preprocess_search_result(&$vars) {
  $vars['info'] = format_date($vars['result']['date'], 'custom', 'F d, Y');
}

/**
 * Views with images need to have a class added in order to get a proper margin on all fields.
 * Checks the view result for field_data_field_ledeimage_node_values if !empty then a class is added to the row.
 * 
 * Usually like to grab the alias so when checking the result of a view. 
 * TODO: Find the lede image by using
 *     - $alias = $vars['view']->field['entity_id_1']->field_alias
 *     - !empty($vars['view']->result[$id]->$alias)
 * This helps when adding fields shifts the view->result array.
 */
function microsite_preprocess_views_view_unformatted(&$vars, $hook) {
  // Usually like to grab the alias so when checking the result of a view
  // Not able to do this as of yet due to everything being called entity_id and that id shifts
  // $alias = $vars['view']->field['entity_id_1']->field_alias;
  foreach($vars['rows'] as $id => $row) {
    if(!empty($vars['view']->result[$id]->field_field_ledeimage) || !empty($vars['view']->result[$id]->field_field_userpicture)) {
      $vars['classes_array'][$id] .= ' has-image';
    }
  }
}

/**
 * The blogauthor_uid is used as a summary argument.
 * When the summary is using this argument we need to filter out when the argument is NULL.
 * We also need to format the argument to return the user name instead of the uid
 * We also need to format the path to use the user->name and not uid.
 */
function microsite_preprocess_views_view_summary(&$vars, $hook) {
  //The following originally comes from the views theme.inc template_preprocess_views_view_summary
  $view = $vars['view'];
  $argument = $view->argument[$view->build_info['summary_level']];

  //If the argument is using the field_blogauthor_uid then change how the summary link is created.
  if($argument->field == 'field_blogauthor_uid') {
   
    $url_options = array();
    
    //This is straight from the views preprocess.
    if (!empty($view->exposed_raw_input)) {
      $url_options['query'] = $view->exposed_raw_input;
    }
    
    $vars['classes'] = array();
    
    foreach ($vars['rows'] as $id => $row) {
      $args = $view->args;
      $args[$argument->position] = $argument->summary_argument($row);
      $name = array();
      if(!empty($args[0])) {
        $base_path = NULL;
        if (!empty($argument->options['summary_options']['base_path'])) {
          $base_path = $argument->options['summary_options']['base_path'];
        }
        $user = user_load($args);
        $name[$id] = $user->name;
        
        $vars['rows'][$id]->link = custom_ldap_name_formatter($user);
        
        $vars['rows'][$id]->url = url($view->get_url($name, $base_path), $url_options);
        $vars['rows'][$id]->count = intval($row->{$argument->count_alias});
        if ($vars['rows'][$id]->url == base_path() . $_GET['q'] || $vars['rows'][$id]->url == base_path() . drupal_get_path_alias($_GET['q'])) {
          $vars['classes'][$id] = 'active';
        }
      }else{
        unset($vars['rows'][$id]);
      }
    }
  }
}

/**
 * Move the file icons to the right of the text instead of in front.
 */
function microsite_file_link($variables) {
  $file = $variables['file'];
  $icon_directory = $variables['icon_directory'];

  $url = file_create_url($file->uri);
  $icon = theme('file_icon', array('file' => $file, 'icon_directory' => $icon_directory));

  // Set options as per anchor format described at
  // http://microformats.org/wiki/file-format-examples
  $options = array(
    'attributes' => array(
      'type' => $file->filemime . '; length=' . $file->filesize,
    ),
  );

  // Use the description as the link text if available.
  if (empty($file->description)) {
    $link_text = $file->filename;
  }
  else {
    $link_text = $file->description;
    $options['attributes']['title'] = check_plain($file->filename);
  }

  return '<span class="file">' . l($link_text, $url, $options) . ' ' . $icon . '</span>';
}

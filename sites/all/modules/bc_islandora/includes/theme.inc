<?php

/**
 * @file
 * Theming functions for Barnard Digital Collections.
 */

/**
 * Generates themed newspaper issue navigator output.
 *
 * @see theme_islandora_newspaper_issue_navigator()
 */
function theme_bc_islandora_newspaper_issue_navigator(&$vars) {
  module_load_include('inc', 'islandora_newspaper', 'includes/utilities');
  $output = '';
  $object = $vars['object'];
  if ($page_links = _barnard_islandora_np_page_pager($object)) {
    $output .= "<p><strong>Pages: </strong>{$page_links}</p>";
  }
  if ($dl_links = _barnard_islandora_dl_links($object)) {
    $output .= '<p><strong>Download: </strong>' . $dl_links . '</p>';
  }
  $newspaper = islandora_newspaper_get_newspaper($object);
  $newspaper = $newspaper ? islandora_object_load($newspaper) : FALSE;
  $issues = $newspaper ? islandora_newspaper_get_issues($newspaper) : [];
  ksort($issues);
  $issues = array_keys($issues);
  if (empty($issues)) {
    $issues[] = $object->id;
  }
  $index = array_search($object->id, $issues);
  $previous_issue = isset($issues[$index - 1]) ? $issues[$index - 1] : NULL;
  $next_issue = isset($issues[$index + 1]) ? $issues[$index + 1] : NULL;
  $links = [];
  if (isset($issues[$index - 1])) {
    $previous_issue = $issues[$index - 1];
    $links[] = [
      'title' => t('Previous issue'),
      'href' => url("islandora/object/{$previous_issue}", ['absolute' => TRUE]),
    ];
  }
  if (isset($issues[$index + 1])) {
    $next_issue = $issues[$index + 1];
    $links[] = [
      'title' => t('Next issue'),
      'href' => url("islandora/object/{$next_issue}", ['absolute' => TRUE]),
    ];
  }
  if ($newspaper) {
    $links[] = [
      'title' => t('All issues'),
      'href' => url('bulletin', ['absolute' => TRUE]),
    ];
  }
  $attributes = ['class' => ['links', 'inline']];
  $output .= theme('links', ['links' => $links, 'attributes' => $attributes]);

  return $output;
}

/**
 * Generates themed newspaper page controls output.
 *
 * @see theme_islandora_newspaper_page_controls()
 */
function theme_bc_islandora_newspaper_page_controls(&$vars) {
  module_load_include('inc', 'islandora', 'includes/datastream');
  module_load_include('inc', 'islandora', 'includes/utilities');
  global $base_url;
  $object = $vars['object'];
  $output = '';
  $page_pager = _barnard_islandora_np_page_pager($object);
  if (!empty($page_pager)) {
    $output .= '<p><strong>Pages:</strong>&nbsp;';
    $output .= $page_pager;
    $output .= '</p>';
  }

  if ($dl_links = _barnard_islandora_dl_links($object)) {
    $output .= '<p><strong>Download: </strong>' . $dl_links . '</p>';
  }

  return $output;
}

/**
 * Generates themed breadcrumb output.
 *
 * @param array $vars
 *   Drupal page array.
 *
 * @return string
 *   Themed breadcrumbs.
 */
function theme_bc_islandora_breadcrumb(array &$vars) {
  // All breadcrumbs begin with a link to go "Home".
  $home_link = l(t('Home'), '/');
  $bc = !in_array($home_link, $vars['breadcrumb']) ? [$home_link] : $vars['breadcrumb'];

  // Dealing with node types other than Islandora.
  if (arg(0) == 'node' && is_numeric(arg(1))) {
    $node = menu_get_object();
    // Barnard Islandora Exhibit Module Breadcrumb Handler.
    if ($node->type === 'exhibition') {
      $bc[] = l(t('Exhibits'), 'exhibits');
      return _barnard_islandora_breadcrumbify($bc);
    }
    // If we aren't a solr collection, stop building and return early.
    elseif ($node->type !== 'islandora_solr_content_type') {
      return NULL;
    }
  }

  // Barnard Islandora Object Breadcrumb Handler.
  if (arg(2) && $object = islandora_object_load(arg(2))) {
    // The first* breadcrumb for any Islandora Object is "Collections".
    // (*not including 'Home' or any predicate breadcrumbs.)
    $bc[] = l(t('Collections'), 'collections');

    // We rely on the local identifier from MODS to map our (Barnard's) Record
    // Groups to objects, and therefore the collections they belong to.
    // @see: https://archives.barnard.edu/finding-materials/collections-list
    //
    // Note we later switch to a PID structure that follows our collection list
    // but continue to rely on the MOD's local identifier for backwards compat.
    $mods = isset($object['MODS']) ? simplexml_load_string($object['MODS']->getContent(NULL)) : NULL;
    $menu_active_title = menu_get_active_title();

    // We cannot do anything more if we don't have mods.
    if (!$mods) {
      $bc[] = $menu_active_title;
      return _barnard_islandora_breadcrumbify($bc);
    }

    // @TODO this is not going to get everything, compound objects and all...
    $current_parent_collection = islandora_get_parents_from_rels_ext($object);
    $current_parent_collection = isset($current_parent_collection[0]) ? $current_parent_collection[0] : 'BC_ISLANDORA-NO-NORMAL-RELATIONSHIP';
    $mods_local_identifier = explode('_', (string) $mods->identifier);
    $barnard_collection_group = preg_replace('/^BC/', '', array_shift($mods_local_identifier));

    switch ($barnard_collection_group) {

      // Barnard Magazine (Administrative Publications - RG BC11-08)
      case '11-08':
        $bc[] = l(t('Barnard Magazine'), 'barnard-magazine');
        $target_collection = 'BC11-08:0';
        $crumb_for_obj_collection = _barnard_islandora_preceding_crumb_from_object($current_parent_collection, $target_collection);
        if (empty($crumb_for_obj_collection)) {
          $bc[] = $menu_active_title;
          break;
        }
        $bc[] = $crumb_for_obj_collection;
        $bc[] = _barnard_islandora_crumb_truncate($menu_active_title);
        break;


      // Mortarboard.
      case '12-01':
        $bc[] = l(t('Yearbook'), 'yearbook');
        $target_collection = variable_get('bc_islandora_yearbook_pid', 'islandora:bookCollection');
        $crumb_for_obj_collection = _barnard_islandora_preceding_crumb_from_object($current_parent_collection, $target_collection);
        if (empty($crumb_for_obj_collection)) {
          $bc[] = $menu_active_title;
          break;
        }
        $bc[] = $crumb_for_obj_collection;
        $bc[] = _barnard_islandora_crumb_truncate($menu_active_title);
        break;

      // Student Publications.
      case '12-09':
      case '12-26':
      case '12-29':
      case '12-30':
        // XX: End of originally titled "special pubs".
      case '12-04':
      case '12-05':
      case '12-06':
      case '12-11':
      case '12-14':
      case '12-17':
      case '12-18':
      case '12-22':
      case '12-23':
      case '12-24':
      case '12-31':
        $bc[] = l(t('Student Publications'), 'student-publications');
        $target_collection = variable_get('bc_islandora_pubs_pid', 'islandora:1022');
        $crumb_for_obj_collection = _barnard_islandora_preceding_crumb_from_object($current_parent_collection, $target_collection);
        if (empty($crumb_for_obj_collection)) {
          $bc[] = $menu_active_title;
          break;
        }
        $bc[] = $crumb_for_obj_collection;
        $bc[] = _barnard_islandora_crumb_truncate($menu_active_title);
        break;

      // 12-03, Bulletin.
      case '12-03':
        $bc[] = l(t('Newspaper'), 'bulletin');
        $target_collection = variable_get('bc_islandora_newspaper_pid', 'islandora:74');
        $crumb_for_obj_collection = _barnard_islandora_preceding_crumb_from_object($current_parent_collection, $target_collection);
        if (empty($crumb_for_obj_collection)) {
          $bc[] = $menu_active_title;
          break;
        }
        $bc[] = $crumb_for_obj_collection;
        $bc[] = _barnard_islandora_crumb_truncate($menu_active_title);
        break;

      // Barnard Center for Research on Women (BC13-58).
      case '13-58':
        $bc[] = l(t('BCRW'), 'bcrw');
        $target_collection = variable_get('bc_islandora_manuscripts_pid', 'islandora:manuscriptCollection');
        $crumb_for_obj_collection = _barnard_islandora_preceding_crumb_from_object($current_parent_collection, $target_collection);
        if (empty($crumb_for_obj_collection)) {
          $bc[] = $menu_active_title;
          break;
        }
        $bc[] = $crumb_for_obj_collection;
        $bc[] = _barnard_islandora_crumb_truncate($menu_active_title);
        break;

      // Barnard Alum Scrapbook and Diary Collection (BC15).
      case '15-02':
      case '15-04':
      case '15-05':
      case '15-06':
      case '15-07':
      case '15-08':
      case '15-09':
      case '15-11':
      case '15-12':
      case '15-14':
      case '15-18':
      case '15-20':
      case '15-28':
        $bc[] = l(t('Alum Scrapbooks'), 'scrapbooks');
        $target_collection = variable_get('bc_islandora_scrapbook_pid', 'BC15:0');
        $crumb_for_obj_collection = _barnard_islandora_preceding_crumb_from_object($current_parent_collection, $target_collection);
        if (empty($crumb_for_obj_collection)) {
          $bc[] = $menu_active_title;
          break;
        }
        $bc[] = $crumb_for_obj_collection;
        // Inclusions of a feather get breadcrumbs together. I was close.
        $compound_parent = $object->relationships->get(FEDORA_RELS_EXT_URI, 'isConstituentOf');
        if (!empty($compound_parent[0]['object']['value']) &&
          $parent = islandora_object_load($compound_parent[0]['object']['value'])) {
          $crumb_for_compound_obj_collection = _barnard_islandora_preceding_crumb_from_object($parent, $target_collection, TRUE, 2);
        }
        if (!empty($crumb_for_compound_obj_collection)) {
          $bc[] = $crumb_for_compound_obj_collection;
        }
        $bc[] = _barnard_islandora_crumb_truncate($menu_active_title, 2);
        break;

      default:
        $genre = $mods && isset($mods->genre) ? (string) $mods->genre : NULL;

        // Handle photographs.
        // Though photographs is typically Barnard's MODS->genre[0], we
        // need to account for other possibilities.
        // This array contains those possibilities: add as necessary.
        // @SEE: http://islandora:8080/solr/collection1/select?q=RELS_EXT_hasModel_uri_s%3A%22info%3Afedora%2Fislandora%3Asp_large_image_cmodel%22&wt=xml&indent=true&facet=true&facet.field=mods_genre_ms&rows=0
        $photograph_mods_genres = [
          'photographs',
          'black-and-white photographs',
          'portraits',
          'color photographs',
        ];

        if (in_array($genre, $photograph_mods_genres)) {
          $bc[] = l(t('Photographs'), 'photographs');
          break;
        }

        // @TODO Maybe revisit this in the future and review.
        // If not a photo, generate links based on relatedItem metadata.
        $links = [];
        foreach ($mods->relatedItem as $ri) {
          if ($host_title = (string) $ri->titleInfo->title) {
            $bc_link = l($host_title, 'islandora/search', [
              'query' => [
                'type' => 'dismax',
                'f[0]' => 'mods_relatedItem_host_titleInfo_title_ms' . ':"' . $host_title . '"',
              ],
            ]);
          }
          // Record group.
          if (isset($ri['displayLabel']) && $ri['displayLabel'] == 'Record Group') {
            $links['rg'] = $bc_link;
          }
          // Collection.
          elseif (isset($bc_link)) {
            $links['c'] = $bc_link;
          }
        }
        // Record group precedes collection in breadcrumb sequence.
        $bc[] = isset($links['rg']) ? $links['rg'] : NULL;
        $bc[] = isset($links['c']) ? $links['c'] : NULL;
        break;
    }

  }
  return _barnard_islandora_breadcrumbify($bc);
}

/**
 * Provides a link to an object but never a link to itself.
 *
 * @param object $object
 *   The Islandora obj to create a link for. Typically a parent collection.
 * @param string $current_object
 *   The ID of the current object. Derp-a-doo!
 * @param bool $truncate
 *   Should the returned link be truncated for space saving?
 * @param int $trunc_len
 *   Length of the truncation to perform on the link before returning it.
 *
 * @return null|string
 *   NULL if the object's ID is a match for the current, meaning it would be a
 *   replication of a previous breadcrumb.
 *   Returns link otherwise.
 *
 * @logical_errors_have_been_made
 * Not going to lie that the logic behind this piece of code is rather backward.
 * I think this is a realization of some backward capability I'm attempting to
 * avoid, but what it is, exactly, I'm not sure.
 * @TODO have a drink, return here.
 */
function _barnard_islandora_preceding_crumb_from_object($object, $current_object, $truncate = FALSE, $trunc_len = 1) {
  // To provide assurances we don't C&B.  There is no type hint.
  if (!is_object($object)) {
    return NULL;
  }
  if ($object->id !== $current_object) {
    $parent_label = $object->label;
    $label = $truncate ? _barnard_islandora_crumb_truncate($parent_label, $trunc_len) : $parent_label;
    $link = "islandora/object/{$object->id}";
    return l(t($label), $link);
  }
  return NULL;
}

/**
 * Helper function returning truncated portions off the end of strings.
 *
 * @param string $string
 *   Incoming string to be truncated from the end.
 * @param int $range
 *   Truncation length from the end of the string in words.
 *
 * @return string
 *   Truncated string of $range, with first letter capitalized.
 */
function _barnard_islandora_crumb_truncate($string, $range = 1) {
  $title_parts = explode(',', $string);
  $ret = array_slice($title_parts, -$range, $range);
  return ucfirst(trim(implode($ret)));
}

/**
 * Helper function to make purdy breadcrumbs.
 *
 * @param array $breadcrumb
 *   An array containing breadcrumbs in order to be displayed.
 * @param string $sep_char
 *   Any sequence of characters to add between breadcrumbs.
 *
 * @return string
 *   HTML representation of the breadcrumbs.
 */
function _barnard_islandora_breadcrumbify(array $breadcrumb, $sep_char = ' » ') {
  return "<div class='breadcrumb'>" . implode($sep_char, $breadcrumb) . "</div>";
}

/**
 * Helper function to generate download links for given datastreams.
 */
function _barnard_islandora_dl_links($obj, $datastreams = ['PDF', 'JPG']) {
  module_load_include('inc', 'islandora', 'includes/datastream');
  $urls = [];
  foreach ($datastreams as $ds) {
    if (isset($obj[$ds])) {
      $ds_label = $obj[$ds]->label;
      $dl_url = islandora_datastream_get_url($obj[$ds], 'download');
      $urls[] = l($ds_label, $dl_url);
    }
    elseif ($ds == 'TRANSCRIPT') {
      $ts_path = "islandora/object/{$obj->id}/download_transcript";
      // We set alias = TRUE here to avoid generating an aliased URL.
      // @see https://api.drupal.org/api/drupal/includes%21common.inc/function/url/7.x
      $urls[] = l(t('Transcript'), $ts_path, ['alias' => TRUE]);
    }
  }

  return implode('<span class="separator">&nbsp;|&nbsp;</span>', $urls);
}

/**
 * Helper function to determine whether $object is a textual document.
 *
 * Called by barnard_theme's template.php.
 */
function _barnard_islandora_is_manuscript($object) {
  module_load_include('inc', 'islandora_paged_content', 'includes/utilities');
  $output = '';
  if (is_object($object->relationships)) {
    $results = $object->relationships->get(FEDORA_RELS_EXT_URI, 'isMemberOfCollection');
    if (!empty($results) && $parent_collection = $results[0]['object']['value']) {
      $collection = islandora_object_load($parent_collection);
      // If this object's parent collection's pid is the same as our database
      // variable bc_islandora_manuscripts_pid, the answer is YES.
      if ($collection->id == variable_get('bc_islandora_manuscripts_pid',
          'islandora:manuscriptCollection')
      ) {
        return TRUE;
      }
    }
  }

  return FALSE;
}

/**
 * Helper function to get the sequential position of an object.
 *
 * Called by barnard_theme's template.php.
 */
function _barnard_islandora_get_sequence($object) {
  $seq_results = $object->relationships->get(ISLANDORA_RELS_EXT_URI, 'isSequenceNumber');
  if (!empty($seq_results)) {
    return $seq_results[0]['object']['value'];
  }

  return NULL;
}

/**
 * Helper function to generate newspaper pager.
 */
function _barnard_islandora_np_page_pager($object) {
  module_load_include('inc', 'islandora_paged_content', 'includes/utilities');

  $results = $object->relationships->get(ISLANDORA_RELS_EXT_URI, 'isPageOf');
  $result = reset($results);
  $is_parent = $result ? FALSE : TRUE;
  $parent = $result ? islandora_object_load($result['object']['value']) : islandora_object_load($object);
  $pages = $parent ? islandora_paged_content_get_pages($parent) : [];
  $pages = array_keys($pages);

  if (empty($pages)) {
    // The page object pid.
    $pages[] = $object->id;
  }
  // The index of the current page in the issue.
  $index = array_search($object->id, $pages);
  $page_links = [];
  foreach ($pages as $i => $pid) {
    $title = $i + 1;
    // Output just a title if we're at the current page.
    if ($index !== FALSE && $i == $index) {
      $page_links[] = $title;
    }
    // Otherwise output a link.
    else {
      $page_links[] = l($title, "islandora/object/{$pid}");
    }
  }
  $page_links[] = $is_parent ? 'View All' : l('View All', "islandora/object/{$parent->id}");

  return implode($page_links, '&nbsp;');
}

/**
 * Generates themed featured object output for the front page.
 *
 * Called by barnard_theme's template.php.
 */
function _barnard_islandora_featured() {
  $output = '';
  // Get a random Featured Object node.
  $node_q = db_select('node', 'n')
    ->fields('n', ['vid'])
    ->condition('type', 'featured_object')
    ->condition('status', 1)
    ->range(0, 1)
    ->orderRandom();
  if ($vid = $node_q->execute()->fetchField()) {
    // Get featured object image.
    $img_q = db_select('file_managed', 'fm')
      ->fields('fm', ['uri']);
    $img_q->join('field_data_field_object_image', 'foi', 'fm.fid = foi.field_object_image_fid');
    $img_q->condition('foi.revision_id', $vid, '=');
    if ($img_uri = $img_q->execute()->fetchField()) {
      drupal_add_js(['featured_img_path' => file_create_url($img_uri)], 'setting');
    }
    // Get caption.
    $cap_q = db_select('field_data_field_caption', 'c')
      ->fields('c', ['field_caption_value'])
      ->condition('revision_id', $vid, '=')
      ->condition('bundle', 'featured_object', '=');
    if ($cap = $cap_q->execute()->fetchField()) {
      // Get pid. !
      $pid_q = db_select('field_data_field_fedora_pid', 'fp')
        ->fields('fp', ['field_fedora_pid_value'])
        ->condition('revision_id', $vid, '=')
        ->condition('bundle', 'featured_object', '=');
      if ($pid = $pid_q->execute()->fetchField()) {
        if ((bool) parse_url($pid, PHP_URL_SCHEME)) {
          $output .= l($cap, "{$pid}");
        }
        else {
          $output .= l($cap, "islandora/object/{$pid}");
        }
      }
      else {
        $output .= $cap;
      }
    }
  }

  return $output;
}

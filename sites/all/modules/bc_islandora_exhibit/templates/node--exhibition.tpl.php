<?php
/**
 * @file
 * Theme implementation to display a node.
 *
 * Available variables:
 * - $title: the (sanitized) title of the node.
 * - $content: An array of node items. Use render($content) to print them all,
 *   or print a subset such as render($content['field_example']). Use
 *   hide($content['field_example']) to temporarily suppress the printing of a
 *   given element.
 * - $user_picture: The node author's picture from user-picture.tpl.php.
 * - $date: Formatted creation date. Preprocess functions can reformat it by
 *   calling format_date() with the desired parameters on the $created variable.
 * - $name: Themed username of node author output from theme_username().
 * - $node_url: Direct url of the current node.
 * - $terms: the themed list of taxonomy term links output from theme_links().
 * - $display_submitted: whether submission information should be displayed.
 * - $classes: String of classes that can be used to style contextually through
 *   CSS. It can be manipulated through the variable $classes_array from
 *   preprocess functions. The default values can be one or more of the
 *   following:
 *   - node: The current template type, i.e., "theming hook".
 *   - node-[type]: The current node type. For example, if the node is a
 *     "Blog entry" it would result in "node-blog". Note that the machine
 *     name will often be in a short form of the human readable label.
 *   - node-teaser: Nodes in teaser form.
 *   - node-preview: Nodes in preview mode.
 *   The following are controlled through the node publishing options.
 *   - node-promoted: Nodes promoted to the front page.
 *   - node-sticky: Nodes ordered above other non-sticky nodes in teaser
 *     listings.
 *   - node-unpublished: Unpublished nodes visible only to administrators.
 *   The following applies only to viewers who are registered users:
 *   - node-by-viewer: Node is authored by the user currently viewing the page.
 * - $title_prefix (array): An array containing additional output populated by
 *   modules, intended to be displayed in front of the main title tag that
 *   appears in the template.
 * - $title_suffix (array): An array containing additional output populated by
 *   modules, intended to be displayed after the main title tag that appears in
 *   the template.
 *
 * Other variables:
 * - $node: Full node object. Contains data that may not be safe.
 * - $type: Node type, i.e. story, page, blog, etc.
 * - $comment_count: Number of comments attached to the node.
 * - $uid: User ID of the node author.
 * - $created: Time the node was published formatted in Unix timestamp.
 * - $classes_array: Array of html class attribute values. It is flattened
 *   into a string within the variable $classes.
 * - $zebra: Outputs either "even" or "odd". Useful for zebra striping in
 *   teaser listings.
 * - $id: Position of the node. Increments each time it's output.
 *
 * Node status variables:
 * - $view_mode: View mode, e.g. 'full', 'teaser'...
 * - $teaser: Flag for the teaser state (shortcut for $view_mode == 'teaser').
 * - $page: Flag for the full page state.
 * - $promote: Flag for front page promotion state.
 * - $sticky: Flags for sticky post setting.
 * - $status: Flag for published status.
 * - $comment: State of comment settings for the node.
 * - $readmore: Flags true if the teaser content of the node cannot hold the
 *   main body content.
 * - $is_front: Flags true when presented in the front page.
 * - $logged_in: Flags true when the current user is a logged-in member.
 * - $is_admin: Flags true when the current user is an administrator.
 *
 * Field variables: for each field instance attached to the node a corresponding
 * variable is defined, e.g. $node->body becomes $body. When needing to access
 * a field's raw values, developers/themers are strongly encouraged to use these
 * variables. Otherwise they will have to explicitly specify the desired field
 * language, e.g. $node->body['en'], thus overriding any language negotiation
 * rule that was previously applied.
 *
 * @see template_preprocess()
 * @see template_preprocess_node()
 * @see zen_preprocess_node()
 * @see template_process()
 */
?>
<div id="node-<?php print $node->nid; ?>" class="<?php print $classes; ?> clearfix"<?php print $attributes; ?>>
  <?php if ($unpublished): ?>
    <div class="unpublished"><?php print t('Unpublished'); ?></div>
  <?php endif; ?>
  <div class="content"<?php print $content_attributes; ?>>
    <?php if (isset($exhibition)): ?>
      <?php print $exhibition; ?>
    <?php endif; ?>
    <div id="ex-themes">
      <?php if (isset($ex_themes)): ?>
        <?php print $ex_themes; ?>
      <?php endif; ?>
    </div>
    <?php foreach ($objects as $i => $object): ?>
      <div class="ex-obj" id="slide<?php print $i + 1; ?>" data-theme="<?php print $object['theme']; ?>" data-layout="<?php print $object['layout'];?>">
        <div class="ex-images">
          <?php if (isset($object['images'])): ?>
            <?php foreach ($object['images'] as $j => $img): ?>
              <div class="large-image" id="<?php print $j + 1; ?>">
                <?php print l(theme('image_style', array(
                      'style_name' => 'exhibit',
                      'path' => $img['uri'],
                    )),
                    variable_get('file_public_path', conf_path() . '/files') . '/' . file_uri_target($img['uri']),
                    array(
                      'attributes' => array(
                        'rel' => 'slide' . strval($i + 1),
                        'class' => 'colorbox-load',
                        'title' => $img['caption'],
                      ),
                      'html' => TRUE,
                    )
                  );
                ?>
                <br />
                <?php // Image caption. ?>
                <span class="caption">
                  <?php print $img['caption']; ?>
                  <?php // Object link (if available). ?>
                  <?php if ($img['pid']): ?>
                    &nbsp;(<?php
                      print l(t('View object'),
                        "islandora/object/{$img['pid']}",
                        array(
                          'attributes' => array('target' => '_blank'),
                        )
                      );
                    ?>)
                  <?php endif; ?>
                </span>
              </div> <?php // .large-image ?>
            <?php endforeach; ?>
          <?php endif; ?>
          <?php // Thumbnails. ?>
          <?php if (isset($object['thumbnail_output'])): ?>
            <div class="ex-thumbnails">
              <?php print $object['thumbnail_output']; ?>
            </div>
          <?php endif; ?>
      </div> <?php // .ex-images ?>
      <div class="ex-txt">
        <?php print $object['description']; ?>
      </div>
      <div id="ex-nav">
        <?php print l(t('« Prev'), request_path(), array(
            'attributes' => array('id' => 'prev'),
            'fragment' => 'page' . ($i ? $i : count($objects)),
          ));
        ?>
        <?php print l(t('Next »'), request_path(), array(
            'attributes' => array('id' => 'next'),
            'fragment' => 'page' . ($i != count($objects) - 1 ? $i + 2 : '1'),
          ));
        ?>
      </div>
    </div>
    <?php endforeach; ?>
    </div> <?php // .ex-obj ?>
  </div> <?php // .content ?>

  <?php print render($content['links']); ?>

  <?php print render($content['comments']); ?>

</div> <!-- /.node -->

<?php
/**
 * @file
 * barnard_content_feature.features.node_export_features.inc
 */

/**
 * Implements hook_node_export_features_default().
 */
function barnard_content_feature_node_export_features_default() {
  $node_export = array(
  'code_string' => 'array(
  (object) array(
      \'vid\' => \'1\',
      \'uid\' => \'1\',
      \'title\' => \'About\',
      \'log\' => \'\',
      \'status\' => \'1\',
      \'comment\' => \'1\',
      \'promote\' => \'0\',
      \'sticky\' => \'0\',
      \'vuuid\' => \'59149393-c2ca-45f3-b2e5-080584b21911\',
      \'nid\' => \'1\',
      \'type\' => \'page\',
      \'language\' => \'en\',
      \'created\' => \'1408373673\',
      \'changed\' => \'1408373673\',
      \'tnid\' => \'0\',
      \'translate\' => \'0\',
      \'uuid\' => \'1329689b-82a6-4db6-abbb-aaf024a3f0ec\',
      \'revision_timestamp\' => \'1408373673\',
      \'revision_uid\' => \'1\',
      \'body\' => array(
        \'und\' => array(
          array(
            \'value\' => \'Lorem ipsum dolor sit amet, consectetur adipiscing elit. Maecenas posuere tincidunt porttitor. Quisque vitae augue pharetra neque scelerisque convallis. Sed lacus mi, commodo sit amet blandit nec, pellentesque non justo. Phasellus nec quam feugiat, consequat lectus ac, commodo lectus. Quisque felis tortor, euismod consectetur porttitor vitae, tempor eget nibh. Pellentesque nec magna vitae magna tincidunt faucibus at id massa. Nam metus lectus, hendrerit vitae metus a, laoreet fringilla diam. Nullam luctus ante at nulla mollis, in tincidunt eros auctor. Fusce quis libero molestie, varius leo et, egestas urna. Mauris vel nibh metus. Integer dictum non nulla nec porttitor. Pellentesque convallis mattis arcu, sed rutrum augue pulvinar ac.\',
            \'summary\' => \'\',
            \'format\' => \'filtered_html\',
            \'safe_value\' => "<p>Lorem ipsum dolor sit amet, consectetur adipiscing elit. Maecenas posuere tincidunt porttitor. Quisque vitae augue pharetra neque scelerisque convallis. Sed lacus mi, commodo sit amet blandit nec, pellentesque non justo. Phasellus nec quam feugiat, consequat lectus ac, commodo lectus. Quisque felis tortor, euismod consectetur porttitor vitae, tempor eget nibh. Pellentesque nec magna vitae magna tincidunt faucibus at id massa. Nam metus lectus, hendrerit vitae metus a, laoreet fringilla diam. Nullam luctus ante at nulla mollis, in tincidunt eros auctor. Fusce quis libero molestie, varius leo et, egestas urna. Mauris vel nibh metus. Integer dictum non nulla nec porttitor. Pellentesque convallis mattis arcu, sed rutrum augue pulvinar ac.</p>\\n",
            \'safe_summary\' => \'\',
          ),
        ),
      ),
      \'rdf_mapping\' => array(
        \'rdftype\' => array(
          \'foaf:Document\',
        ),
        \'title\' => array(
          \'predicates\' => array(
            \'dc:title\',
          ),
        ),
        \'created\' => array(
          \'predicates\' => array(
            \'dc:date\',
            \'dc:created\',
          ),
          \'datatype\' => \'xsd:dateTime\',
          \'callback\' => \'date_iso8601\',
        ),
        \'changed\' => array(
          \'predicates\' => array(
            \'dc:modified\',
          ),
          \'datatype\' => \'xsd:dateTime\',
          \'callback\' => \'date_iso8601\',
        ),
        \'body\' => array(
          \'predicates\' => array(
            \'content:encoded\',
          ),
        ),
        \'uid\' => array(
          \'predicates\' => array(
            \'sioc:has_creator\',
          ),
          \'type\' => \'rel\',
        ),
        \'name\' => array(
          \'predicates\' => array(
            \'foaf:name\',
          ),
        ),
        \'comment_count\' => array(
          \'predicates\' => array(
            \'sioc:num_replies\',
          ),
          \'datatype\' => \'xsd:integer\',
        ),
        \'last_activity\' => array(
          \'predicates\' => array(
            \'sioc:last_activity_date\',
          ),
          \'datatype\' => \'xsd:dateTime\',
          \'callback\' => \'date_iso8601\',
        ),
      ),
      \'cid\' => \'0\',
      \'last_comment_timestamp\' => \'1408373673\',
      \'last_comment_name\' => NULL,
      \'last_comment_uid\' => \'1\',
      \'comment_count\' => \'0\',
      \'name\' => \'islandora\',
      \'picture\' => \'0\',
      \'data\' => \'b:0;\',
      \'path\' => array(
        \'pid\' => \'1\',
        \'source\' => \'node/1\',
        \'alias\' => \'about\',
        \'language\' => \'en\',
      ),
      \'menu\' => array(
        \'link_title\' => \'About\',
        \'mlid\' => 0,
        \'plid\' => \'0\',
        \'menu_name\' => \'main-menu\',
        \'weight\' => \'-46\',
        \'options\' => array(),
        \'module\' => \'menu\',
        \'expanded\' => \'0\',
        \'hidden\' => \'0\',
        \'has_children\' => \'0\',
        \'customized\' => 0,
        \'parent_depth_limit\' => 8,
        \'description\' => \'\',
        \'enabled\' => 1,
      ),
      \'node_export_drupal_version\' => \'7\',
    ),
)',
);
  return $node_export;
}

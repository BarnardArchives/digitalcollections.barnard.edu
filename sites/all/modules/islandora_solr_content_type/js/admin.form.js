/**
 * @see menu.admin.js from core module Menu.
 * This is copy pasta.
 */
(function ($) {

    Drupal.behaviors.isctMenuChangeParentItems = {
        attach: function (context, settings) {
            $('div#edit-menu-options-islandora-solr-content-type').each(function () {
                $(this).change(function () {
                    // Update list of available parent menu items.
                    Drupal.isct_menu_update_parent_list();
                });
            });
        }
    };

    /**
     * Function to set the options of the menu parent item dropdown.
     */
    Drupal.isct_menu_update_parent_list = function () {
        var values = [];

        $('input:checked', $('div#edit-menu-options-islandora-solr-content-type')).each(function () {
            // Get the names of all checked menus.
            values.push(Drupal.checkPlain($.trim($(this).val())));
        });

        var url = Drupal.settings.basePath + 'admin/structure/menu/parents';
        $.ajax({
            url: location.protocol + '//' + location.host + url,
            type: 'POST',
            data: {'menus[]': values},
            dataType: 'json',
            success: function (options) {
                // Save key of last selected element.
                var selected = $('select#edit-menu-parent-islandora-solr-content-type :selected').val();
                // Remove all exisiting options from dropdown.
                $('select#edit-menu-parent-islandora-solr-content-type').children().remove();
                // Add new options to dropdown.
                jQuery.each(options, function (index, value) {
                    $('select#edit-menu-parent-islandora-solr-content-type').append(
                        $('<option ' + (index == selected ? ' selected="selected"' : '') + '></option>').val(index).text(value)
                    );
                });
            }
        });
    };

})(jQuery);
/*global
    Drupal, jQuery, window
*/

(function ($) {
  Drupal.behaviors.bc_islandora = {
    collections_lp: {
      listeners: function () {
        $('.landingpage-icon').click(function () {
          window.location = $(this).find('a').attr('href');
        });
      },
      run: function () {
        if (window.location.pathname === '/collections') {
          this.listeners();
        }
      }
    },
    // Front page feature.
    // Assumes that settings.featured_img_path will be set (via backend).
    front_featured: {
      run: function (settings) {
        if (settings.featured_img_path !== undefined && settings.featured_img_path.length !== 0) {
          $('body.front').css('background', 'url("' + settings.featured_img_path + '") 50% 50% no-repeat fixed');
        }
      }
    },
    attach: function (context, settings) {
      this.collections_lp.run();
      this.front_featured.run(settings);
    }
  };
}(jQuery));

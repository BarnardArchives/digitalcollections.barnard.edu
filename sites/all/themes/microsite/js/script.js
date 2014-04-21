(function ($) {

Drupal.behaviors.bernard_theme = {
  attach: function (context, settings) {
    // This behavior attaches by ID, so is only valid once on a page.
    if ($('.main-menu-secondary-trigger.bernard-theme-processed').size()) {
      return;
    }
    $('.main-menu-secondary-trigger', context).addClass('bernard-theme-processed');

    // Show and hide the secondary navigation.
    $('.main-menu-secondary-trigger', context).toggle(
      function () {
        $(this).addClass('collapsable').html(Drupal.t('Collapse'));
        $('#navigation .content ul ul').slideDown('fast');
      },
      function () {
        $(this).removeClass('collapsable').html(Drupal.t('Expand'));
        $('#navigation .content ul ul').slideUp('fast');
      }
    );
  }
};

Drupal.behaviors.exposed_filters_collapse = {
  attach: function (context, settings) {
    $('.block-views .content .views-exposed-form').parents('.block-views').addClass('views-filter');
    // Show and hide the secondary navigation.
    $('.block-views.views-filter h2.block-title').toggle(
      function () {
        $(this).addClass('collapsible');
    	$(this).parent().find('.content').slideDown('fast');
      },
      function () {
        $(this).removeClass('collapsible');
		$(this).parent().find('.content').slideUp('fast');
      }
    );
  }
};

})(jQuery);

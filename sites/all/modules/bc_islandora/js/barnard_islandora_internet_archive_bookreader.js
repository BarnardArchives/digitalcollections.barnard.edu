/**
 * @file
 * Defines initializing/attaching the Book Reader to the
 * defined element.
 *
 * This bookreader file is modified from its original form.
 * Purpose: redefines the BookReader proto to be aware
 * of Islandora Compound Objects.
 */

(function ($) {
  Drupal.behaviors.islandoraInternetArchiveBookReader = {
    attach: function (context, settings) {
      $('.islandora-internet-archive-bookreader', context).once('islandora-bookreader', function () {
        var bookReader = new IslandoraBookReader(settings.islandoraInternetArchiveBookReader);
        // Initialize and Render the BookReader.
        bookReader.init();

        // Hide unused or unwanted toolbar buttons from view.
        $('#BRtoolbar').find('.read, .info, .share, .play, .pause').hide();

        if (!bookReader.searchEnabled()) {
          $('#textSrch').hide();
          $('#btnSrch').hide();
        }

        // Handle page resize, required for full screen.
        $(window).resize(function () {
          bookReader.windowResize();
          bookReader.updateCompoundBlockLocation();
        });

        if ($.browser.mobile && settings.islandoraInternetArchiveBookReader.mobilize) {
          bookReader.goFullScreen();
        }
      });
    }
  };
})(jQuery);

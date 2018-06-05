/*global
    Drupal, jQuery, location
*/

(function ($) {
    Drupal.behaviors.bc_islandora_exhibit = {
        set_firsts: function() {
            $('.ex-obj').first().addClass('first');
            $('.ex-images').first().addClass('first');
            $('.ex-images').each(function() {
                $(this).find('.large-image').first().addClass('first');
            });
        },
        // Show the exhibit's first slide.
        show_first: function ($selector) {
            $selector.each(function () {
                if (!$(this).hasClass('first')) {
                    $(this).hide();
                } else {
                    $(this).addClass('active');
                }
            });
        },
        show_slide: function ($slide) {
            $slide.show().addClass('active');
            $slide.find('.large-image:first').show().addClass('active');
            $slide.find('.ex-thumbnails img:first').addClass('active');
        },
        hide_slide: function ($slide) {
            $slide.hide().removeClass('active');
            $slide.find('.large-image.active').hide().removeClass('active');
            $slide.find('.ex-thumbnails img.active').removeClass('active');
        },
        toggle_slide: function ($from_slide, $to_slide) {
            var self = this;
            self.hide_slide($from_slide);
            self.show_slide($to_slide);
        },
        // Indicate that a theme is active (current active theme becomes
        // inactive).
        activate_theme: function (theme) {
            var $theme = $('#ex-themes .theme[data-theme="' + theme + '"]'),
                $active_theme = $('#ex-themes .theme.active'),
                active_theme = $active_theme.innerHTML;
            if (active_theme !== theme) {
                $active_theme.removeClass('active');
                $theme.addClass('active');
            }
        },
        pager_click: function (pageNumber, self) {
            var $active_slide = $('.ex-obj.active'),
                $slide_to = $('.ex-obj#slide' + pageNumber);
            self.toggle_slide($active_slide, $slide_to);
            self.activate_theme($slide_to.attr('data-theme'));
        },
        // Show the slide indicated by the hash parameter.
        show_hash: function (hash) {
            var self = this,
                page = parseInt(hash.split('#page-')[1], 10);
            self.pager_click(page, {}, self);
        },
        listeners: function () {
            var self = this;
            // Theme click.
            $('#ex-themes .theme').click(function () {
                var theme = this.innerHTML,
                    $theme_first = $('.ex-obj[data-theme="' + theme + '"]:first'),
                    $active_obj = $('.ex-obj.active');
                if (!$theme_first.hasClass('active')) {
                    self.toggle_slide($active_obj, $theme_first);
                }
                if (!$(this).hasClass('active')) {
                    $(this).parent().find('.active').removeClass('active');
                    $(this).addClass('active');
                }
            });
            // Thumbnail click.
            $('.ex-thumbnails a').click(function () {
                // The "id" of the clicked thumbnail.
                var active_img_id = $(this).find('img').attr('id').split('tn')[1];
                // The current "active" image: hide it.
                var $active_img = $(this).parent().parent().find('.large-image.active');
                $active_img.hide().removeClass('active');
                // The image corresponding to the clicked thumbnail: show it.
                var $clicked_img = $(this).parent().parent().find('#' + active_img_id);
                $clicked_img.show().addClass('active');
                // Toggle "active" thumbnails.
                $(this).parent().find('img.active').removeClass('active');
                $(this).find('img').addClass('active');
            });
            // Nav click.
            $('#ex-nav a').click(function () {
                var page_to = $(this)[0].hash.split('#page')[1];
                self.pager_click(page_to, self);
            });
        },
        attach: function (context, settings) {
            var self = this;
            self.set_firsts();
            $('.ex-images').each(function () {
                self.show_first($(this).find('.large-image'));
            });
            self.show_first($('.ex-obj'));
            $('.ex-thumbnails').each(function () {
                var active_img = $('.ex-images .large-image.first').attr('id');
                $('.ex-thumbnails img#tn' + active_img).addClass('active');
            });
            if (location.hash.length > 0) {
                var frag = location.hash.split('#'),
                    slide_id = frag[1].replace('page', 'slide');
                self.toggle_slide($('.ex-obj.active'), $('.ex-obj#' + slide_id));
                // Is thumbnail fragment set?
                if (frag.length > 2) {
                    $('.ex-obj#' + slide_id + ' .ex-thumbnails img#' + frag[2]).parent().click();
                }
            }
            self.activate_theme($('.ex-obj.active').attr('data-theme'));
            self.listeners();
        }
    };
}(jQuery));
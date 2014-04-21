(function ($) {
  $(document).ready(function () {
    var bulletin_calendar = {
        hide_lists: function () {
            $('.bulletin-calendar ul').each(function() {
                if ($(this).parent().attr('class') != 'years') {
                    $(this).addClass('inactive');
                }
                else {
                    $(this).addClass('active');
                }
            });
        },
        listeners: function () {
            var self = this;
            // year click
            $('.years li.year a').click(function () {
                var year = $(this).parent().attr('id');
                console.log('got year: ' + year);
                var months = $('.months-' + year);
                self.toggle_list('ul.years', '.months-' + year);
                self.nav_display('.year #nav', year);
            });
            // month click
            $('.months li.month a').click(function () {
                var year = $('.bulletin-nav .year #nav').text();
                var month = $(this).parent().attr('id');
                var month_text = $(this).text();
                var issues_selector = '.issues-' + year + '-' + month;
                self.toggle_list('.months-' + year, issues_selector);
                self.nav_display('.month', month_text + ', ');
            });
            // nav year click
            $('.bulletin-nav .year #nav').click(function () {
                var active_list = $('.bulletin-calendar').find('ul.active');
                var month_list = $('ul.months-' + $(this).text());
                self.toggle_list(active_list, month_list);
                self.nav_display('.month', '');
            });
        },
        nav_display: function (selector, text) {
            $('.bulletin-nav').find(selector).html(text);
        },
        toggle_list: function (from_selector, to_selector) {
            $(from_selector).removeClass('active').addClass('inactive');
            $(to_selector).removeClass('inactive').addClass('active');
        },
        run: function () {
            this.hide_lists();
            this.listeners();
        }
    };

    bulletin_calendar.run();
  });
}) (jQuery);;

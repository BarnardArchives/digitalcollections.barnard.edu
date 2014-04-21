(function ($) {

/**
 * Attaches double-click behavior to toggle full path of Krumo elements.
 */
Drupal.behaviors.devel = {
  attach: function (context, settings) {

    // Add hint to footnote
    $('.krumo-footnote .krumo-call').once().before('<img style="vertical-align: middle;" title="Click to expand. Double-click to show path." src="' + settings.basePath + 'misc/help.png"/>');

    var krumo_name = [];
    var krumo_type = [];

    function krumo_traverse(el) {
      krumo_name.push($(el).html());
      krumo_type.push($(el).siblings('em').html().match(/\w*/)[0]);

      if ($(el).closest('.krumo-nest').length > 0) {
        krumo_traverse($(el).closest('.krumo-nest').prev().find('.krumo-name'));
      }
    }

    $('.krumo-child > div:first-child', context).dblclick(
      function(e) {
        if ($(this).find('> .krumo-php-path').length > 0) {
          // Remove path if shown.
          $(this).find('> .krumo-php-path').remove();
        }
        else {
          // Get elements.
          krumo_traverse($(this).find('> a.krumo-name'));

          // Create path.
          var krumo_path_string = '';
          for (var i = krumo_name.length - 1; i >= 0; --i) {
            // Start element.
            if ((krumo_name.length - 1) == i)
              krumo_path_string += '$' + krumo_name[i];

            if (typeof krumo_name[(i-1)] !== 'undefined') {
              if (krumo_type[i] == 'Array') {
                krumo_path_string += "[";
                if (!/^\d*$/.test(krumo_name[(i-1)]))
                  krumo_path_string += "'";
                krumo_path_string += krumo_name[(i-1)];
                if (!/^\d*$/.test(krumo_name[(i-1)]))
                  krumo_path_string += "'";
                krumo_path_string += "]";
              }
              if (krumo_type[i] == 'Object')
                krumo_path_string += '->' + krumo_name[(i-1)];
            }
          }
          $(this).append('<div class="krumo-php-path" style="font-family: Courier, monospace; font-weight: bold;">' + krumo_path_string + '</div>');

          // Reset arrays.
          krumo_name = [];
          krumo_type = [];
        }
      }
    );
  }
};

})(jQuery);
;
Drupal.behaviors.xmlFormElementTabs = {
  tabs: {
    tool_tip: null,
    tabs: null, // Collection of all tabpanels.
    collapsibleTabs: null,
    nonCollapsibleTabs: null,
    loadPanels: function(collapse, context) {
      var load = '.xml-form-elements-tabs:not(.processed)';
      var collapsible = '.xml-form-elements-tabs-collapsible';
      var collapsed = '.xml-form-elements-tabs-collapsed';
      this.tabs = jQuery(load);
      this.collapsibleTabs = this.tabs.filter(collapsible);
      this.nonCollapsibleTabs = this.tabs.not(collapsible);
      var expandedTabs = this.collapsibleTabs.not(collapsed);
      var collapsedTabs = this.collapsibleTabs.filter(collapsed);
      if (collapsedTabs.length > 0) {
        collapsedTabs.tabs({
          collapsible: true,
          selected: collapse ? -1 : undefined,
          select: this.setCollapsibleIconOnSelect,
          create: this.setCollapsibleIconOnCreate
        });
      }
      if (expandedTabs.length > 0) {
        expandedTabs.tabs({
          collapsible: true,
          select: this.setCollapsibleIconOnSelect,
          create: this.setCollapsibleIconOnCreate
        });
      }
      if (this.nonCollapsibleTabs.length > 0) {
        this.nonCollapsibleTabs.tabs({});
      }
      this.tabs.each(function() {
        jQuery(this).tabs({
          selected: jQuery(this).find('li').length - 1
        });
      });
    },
    setCollapsibleIconOnSelect: function(event, ui) {
      var icon = jQuery('span.expand-tabpanel-icon:first', this);
      if (jQuery(ui.panel).hasClass('ui-tabs-hide')) {
        icon.removeClass('ui-icon-circle-triangle-e');
        icon.addClass('ui-icon-circle-triangle-s');
      }
      else {
        icon.removeClass('ui-icon-circle-triangle-s');
        icon.addClass('ui-icon-circle-triangle-e');
      }
    },
    setCollapsibleIconOnCreate: function(event, ui) {
      var icon = jQuery('span.expand-tabpanel-icon:first', this);
      if (jQuery('div.ui-tabs-panel:not(.ui-tabs-hide)', this).length > 0) {
        icon.removeClass('ui-icon-circle-triangle-e');
        icon.addClass('ui-icon-circle-triangle-s');
      }
      else {
        icon.removeClass('ui-icon-circle-triangle-s');
        icon.addClass('ui-icon-circle-triangle-e');
      }
    },
    attachToolTips: function() {
      jQuery('.tool_tip_trigger').once(function() {
        jQuery(this).hover(function(e) {
          var html = '';// + i + '<br/>';
          var id = jQuery(this).children('a[href]').attr('href');
          jQuery(id + ' div.form-item').each(function() {
            var item = jQuery(this);
            jQuery('> input[type~="text"]', item).each(function(i, text) {
              var id = jQuery(text).attr('id');
              var label = jQuery('label[for="' + id + '"]', item);
              if (label.length > 0) {
                label = label.text().trim();
                var textOut = jQuery(text).val();
                jQuery('input[class~="form-tag"]', jQuery(text).parent()).each(function() {
                  var tag = jQuery(this);
                  textOut += ' ' + tag.val();
                });
                textOut = jQuery.trim(textOut);
                if (textOut.length > 0) {
                  html += label + ': ' + textOut + '<br/>';
                }
              }
            });

            jQuery('> select', item).each(function(index, select) {
              var id = jQuery(select).attr('id');
              var label = jQuery('label[for=' + id + ']');
              if (label.length > 0) {
                label = label.text().trim();
                html += label + ': ';
              }
              jQuery('option:selected', select).each(function(idx, selected) {
                html += jQuery(selected).text().trim() + '<br/>';
              });
            });
          });
          html = jQuery.trim(html);
          if (html == "") {
            html = Drupal.t("Empty");
          }

          if (Drupal.behaviors.xmlFormElementTabs.tabs.tool_tip != null) {
            Drupal.behaviors.xmlFormElementTabs.tabs.tool_tip.remove();
          }
          else {
            Drupal.behaviors.xmlFormElementTabs.tabs.tool_tip = jQuery(document.createElement('span')).addClass('tool_tip');
          }

          Drupal.behaviors.xmlFormElementTabs.tabs.tool_tip.html(html);

          var x = e.pageX + 20,
              y = e.pageY + 20,
              w = Drupal.behaviors.xmlFormElementTabs.tabs.tool_tip.width(),
              h = Drupal.behaviors.xmlFormElementTabs.tabs.tool_tip.height(),
              dx = jQuery(window).width() - (x + w),
              dy = jQuery(window).height() - (y + h);
          if (dx < 20)
            x = e.pageX - w - 20;
          if (dy < 20)
            y = e.pageY - h - 20;
          Drupal.behaviors.xmlFormElementTabs.tabs.tool_tip.css({
            'left': x,
            'top': y
          });

          Drupal.behaviors.xmlFormElementTabs.tabs.tool_tip.appendTo('body');
        },
            function() {
              if (Drupal.behaviors.xmlFormElementTabs.tabs.tool_tip != null) {
                Drupal.behaviors.xmlFormElementTabs.tabs.tool_tip.remove();
              }
            });
      });
    },
    enableActions: function() {
      var icons = jQuery(".ui-icon-close:not(.processed)");
      icons.click(function() {
        jQuery("#" + jQuery(this).text()).trigger("mousedown");
      });
      icons.addClass('processed');
    }
  },
  attach: function(context, settings) {
    this.tabs.loadPanels(true, context);
    this.tabs.attachToolTips();
    this.tabs.enableActions();

  }
}
;
Drupal.behaviors.xmlFormElementTags = function(context) {
  $('.xml-form-elements-tags .tag-editor:not(.processed)').keypress(function(e) {
    if (e.keyCode == '13') {
      e.preventDefault();
      var id = this.id;
      $('#' + id + '-add').trigger("mousedown");
      $('.xml-form-elements-tags .tag-editor').addClass('processed');
    }
  });
}


;
(function ($) {

/**
 * A progressbar object. Initialized with the given id. Must be inserted into
 * the DOM afterwards through progressBar.element.
 *
 * method is the function which will perform the HTTP request to get the
 * progress bar state. Either "GET" or "POST".
 *
 * e.g. pb = new progressBar('myProgressBar');
 *      some_element.appendChild(pb.element);
 */
Drupal.progressBar = function (id, updateCallback, method, errorCallback) {
  var pb = this;
  this.id = id;
  this.method = method || 'GET';
  this.updateCallback = updateCallback;
  this.errorCallback = errorCallback;

  // The WAI-ARIA setting aria-live="polite" will announce changes after users
  // have completed their current activity and not interrupt the screen reader.
  this.element = $('<div class="progress" aria-live="polite"></div>').attr('id', id);
  this.element.html('<div class="bar"><div class="filled"></div></div>' +
                    '<div class="percentage"></div>' +
                    '<div class="message">&nbsp;</div>');
};

/**
 * Set the percentage and status message for the progressbar.
 */
Drupal.progressBar.prototype.setProgress = function (percentage, message) {
  if (percentage >= 0 && percentage <= 100) {
    $('div.filled', this.element).css('width', percentage + '%');
    $('div.percentage', this.element).html(percentage + '%');
  }
  $('div.message', this.element).html(message);
  if (this.updateCallback) {
    this.updateCallback(percentage, message, this);
  }
};

/**
 * Start monitoring progress via Ajax.
 */
Drupal.progressBar.prototype.startMonitoring = function (uri, delay) {
  this.delay = delay;
  this.uri = uri;
  this.sendPing();
};

/**
 * Stop monitoring progress via Ajax.
 */
Drupal.progressBar.prototype.stopMonitoring = function () {
  clearTimeout(this.timer);
  // This allows monitoring to be stopped from within the callback.
  this.uri = null;
};

/**
 * Request progress data from server.
 */
Drupal.progressBar.prototype.sendPing = function () {
  if (this.timer) {
    clearTimeout(this.timer);
  }
  if (this.uri) {
    var pb = this;
    // When doing a post request, you need non-null data. Otherwise a
    // HTTP 411 or HTTP 406 (with Apache mod_security) error may result.
    $.ajax({
      type: this.method,
      url: this.uri,
      data: '',
      dataType: 'json',
      success: function (progress) {
        // Display errors.
        if (progress.status == 0) {
          pb.displayError(progress.data);
          return;
        }
        // Update display.
        pb.setProgress(progress.percentage, progress.message);
        // Schedule next timer.
        pb.timer = setTimeout(function () { pb.sendPing(); }, pb.delay);
      },
      error: function (xmlhttp) {
        pb.displayError(Drupal.ajaxError(xmlhttp, pb.uri));
      }
    });
  }
};

/**
 * Display errors on the page.
 */
Drupal.progressBar.prototype.displayError = function (string) {
  var error = $('<div class="messages error"></div>').html(string);
  $(this.element).before(error).hide();

  if (this.errorCallback) {
    this.errorCallback(this);
  }
};

})(jQuery);
;
(function ($) {

Drupal.behaviors.textarea = {
  attach: function (context, settings) {
    $('.form-textarea-wrapper.resizable', context).once('textarea', function () {
      var staticOffset = null;
      var textarea = $(this).addClass('resizable-textarea').find('textarea');
      var grippie = $('<div class="grippie"></div>').mousedown(startDrag);

      grippie.insertAfter(textarea);

      function startDrag(e) {
        staticOffset = textarea.height() - e.pageY;
        textarea.css('opacity', 0.25);
        $(document).mousemove(performDrag).mouseup(endDrag);
        return false;
      }

      function performDrag(e) {
        textarea.height(Math.max(32, staticOffset + e.pageY) + 'px');
        return false;
      }

      function endDrag(e) {
        $(document).unbind('mousemove', performDrag).unbind('mouseup', endDrag);
        textarea.css('opacity', 1);
      }
    });
  }
};

})(jQuery);
;

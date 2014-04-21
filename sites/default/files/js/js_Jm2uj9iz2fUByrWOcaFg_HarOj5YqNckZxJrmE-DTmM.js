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
(function(t,e){if(typeof exports=="object")module.exports=e();else if(typeof define=="function"&&define.amd)define(e);else t.Spinner=e()})(this,function(){"use strict";var t=["webkit","Moz","ms","O"],e={},i;function o(t,e){var i=document.createElement(t||"div"),o;for(o in e)i[o]=e[o];return i}function n(t){for(var e=1,i=arguments.length;e<i;e++)t.appendChild(arguments[e]);return t}var r=function(){var t=o("style",{type:"text/css"});n(document.getElementsByTagName("head")[0],t);return t.sheet||t.styleSheet}();function s(t,o,n,s){var a=["opacity",o,~~(t*100),n,s].join("-"),f=.01+n/s*100,l=Math.max(1-(1-t)/o*(100-f),t),u=i.substring(0,i.indexOf("Animation")).toLowerCase(),d=u&&"-"+u+"-"||"";if(!e[a]){r.insertRule("@"+d+"keyframes "+a+"{"+"0%{opacity:"+l+"}"+f+"%{opacity:"+t+"}"+(f+.01)+"%{opacity:1}"+(f+o)%100+"%{opacity:"+t+"}"+"100%{opacity:"+l+"}"+"}",r.cssRules.length);e[a]=1}return a}function a(e,i){var o=e.style,n,r;i=i.charAt(0).toUpperCase()+i.slice(1);for(r=0;r<t.length;r++){n=t[r]+i;if(o[n]!==undefined)return n}if(o[i]!==undefined)return i}function f(t,e){for(var i in e)t.style[a(t,i)||i]=e[i];return t}function l(t){for(var e=1;e<arguments.length;e++){var i=arguments[e];for(var o in i)if(t[o]===undefined)t[o]=i[o]}return t}function u(t){var e={x:t.offsetLeft,y:t.offsetTop};while(t=t.offsetParent)e.x+=t.offsetLeft,e.y+=t.offsetTop;return e}function d(t,e){return typeof t=="string"?t:t[e%t.length]}var p={lines:12,length:7,width:5,radius:10,rotate:0,corners:1,color:"#000",direction:1,speed:1,trail:100,opacity:1/4,fps:20,zIndex:2e9,className:"spinner",top:"auto",left:"auto",position:"relative"};function c(t){if(typeof this=="undefined")return new c(t);this.opts=l(t||{},c.defaults,p)}c.defaults={};l(c.prototype,{spin:function(t){this.stop();var e=this,n=e.opts,r=e.el=f(o(0,{className:n.className}),{position:n.position,width:0,zIndex:n.zIndex}),s=n.radius+n.length+n.width,a,l;if(t){t.insertBefore(r,t.firstChild||null);l=u(t);a=u(r);f(r,{left:(n.left=="auto"?l.x-a.x+(t.offsetWidth>>1):parseInt(n.left,10)+s)+"px",top:(n.top=="auto"?l.y-a.y+(t.offsetHeight>>1):parseInt(n.top,10)+s)+"px"})}r.setAttribute("role","progressbar");e.lines(r,e.opts);if(!i){var d=0,p=(n.lines-1)*(1-n.direction)/2,c,h=n.fps,m=h/n.speed,y=(1-n.opacity)/(m*n.trail/100),g=m/n.lines;(function v(){d++;for(var t=0;t<n.lines;t++){c=Math.max(1-(d+(n.lines-t)*g)%m*y,n.opacity);e.opacity(r,t*n.direction+p,c,n)}e.timeout=e.el&&setTimeout(v,~~(1e3/h))})()}return e},stop:function(){var t=this.el;if(t){clearTimeout(this.timeout);if(t.parentNode)t.parentNode.removeChild(t);this.el=undefined}return this},lines:function(t,e){var r=0,a=(e.lines-1)*(1-e.direction)/2,l;function u(t,i){return f(o(),{position:"absolute",width:e.length+e.width+"px",height:e.width+"px",background:t,boxShadow:i,transformOrigin:"left",transform:"rotate("+~~(360/e.lines*r+e.rotate)+"deg) translate("+e.radius+"px"+",0)",borderRadius:(e.corners*e.width>>1)+"px"})}for(;r<e.lines;r++){l=f(o(),{position:"absolute",top:1+~(e.width/2)+"px",transform:e.hwaccel?"translate3d(0,0,0)":"",opacity:e.opacity,animation:i&&s(e.opacity,e.trail,a+r*e.direction,e.lines)+" "+1/e.speed+"s linear infinite"});if(e.shadow)n(l,f(u("#000","0 0 4px "+"#000"),{top:2+"px"}));n(t,n(l,u(d(e.color,r),"0 0 1px rgba(0,0,0,.1)")))}return t},opacity:function(t,e,i){if(e<t.childNodes.length)t.childNodes[e].style.opacity=i}});function h(){function t(t,e){return o("<"+t+' xmlns="urn:schemas-microsoft.com:vml" class="spin-vml">',e)}r.addRule(".spin-vml","behavior:url(#default#VML)");c.prototype.lines=function(e,i){var o=i.length+i.width,r=2*o;function s(){return f(t("group",{coordsize:r+" "+r,coordorigin:-o+" "+-o}),{width:r,height:r})}var a=-(i.width+i.length)*2+"px",l=f(s(),{position:"absolute",top:a,left:a}),u;function p(e,r,a){n(l,n(f(s(),{rotation:360/i.lines*e+"deg",left:~~r}),n(f(t("roundrect",{arcsize:i.corners}),{width:o,height:i.width,left:i.radius,top:-i.width>>1,filter:a}),t("fill",{color:d(i.color,e),opacity:i.opacity}),t("stroke",{opacity:0}))))}if(i.shadow)for(u=1;u<=i.lines;u++)p(u,-2,"progid:DXImageTransform.Microsoft.Blur(pixelradius=2,makeshadow=1,shadowopacity=.3)");for(u=1;u<=i.lines;u++)p(u);return n(e,l)};c.prototype.opacity=function(t,e,i,o){var n=t.firstChild;o=o.shadow&&o.lines||0;if(n&&e+o<n.childNodes.length){n=n.childNodes[e+o];n=n&&n.firstChild;n=n&&n.firstChild;if(n)n.opacity=i}}}var m=f(o("group"),{behavior:"url(#default#VML)"});if(!a(m,"transform")&&m.adj)h();else i=a(m,"animation");return c});;
/**
 * @file
 * Triggers the display of a spinning icon when the form is submitted.
 */
(function ($) {

  Drupal.behaviors.spinner = {
    attach: function(context, settings) {
      // Store what triggered the submit.
      $('form').once('submit-resolver', function() {
        $(this).click(function(event) {
          $(this).data('clicked', $(event.target));
        });
        $(this).keypress(function(event) {
          // On enter the first submit button is assumed as is most often the
          // case and this is part of the HTML 5 specification, although some
          // Browsers may choose the button with the lowest tab-index.
          if (event.which == 13) {
            $(this).data('clicked', $(':submit', this).first());
          }
        });
      });
      for (var base in settings.spinner) {
        var id = '#' + base;
        $(id, context).once('spinner', function () {
          var spinner = new Spinner(settings.spinner[base].opts);
          $(id).parents('form').one('submit', function(event) {
            if ($(this).data('clicked').is(id)) {
              event.preventDefault();
              // Add Message.
              var message = $('<div/>').text(settings.spinner[base].message);
              $(id).after(message);
              // Make UI changes.
              spinner.spin(this);
              $('#edit-next').hide();
              $('#edit-prev').hide(); 
              // Submit the form after a set timeout, this handles problems with
              // safari, in that safari submit's immediately..
              if (navigator.userAgent.indexOf('Safari') != -1 && navigator.userAgent.indexOf('Chrome') == -1) { 
                $(':submit').attr('disabled', 'disabled');
              }
              setTimeout(function() {
                // Allow for the button to be clicked, then click it then
                // prevent the default behavoir.
                $(id).removeAttr('disabled')
                  .click()
                  .click(function(event) {
                    event.preventDefault();
                  });
              }, 500);
            }
            return true;
          });
        });
      }
    }
  };
})(jQuery);
;

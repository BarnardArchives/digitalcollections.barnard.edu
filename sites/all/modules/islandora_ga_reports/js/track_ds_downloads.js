(function ($) {

  Drupal.behaviors.islandoraGATrack = {
    attach: function (context, settings) {
      $('td.datastream-download a').click(function() {
        var url = $(this).attr('href');
        if (typeof _gaq != null && typeof _gaq != "undefined") {
          _gaq.push(['_trackPageview', url]);
        }
        else if (typeof ga != null && typeof ga != "undefined") {
          ga('send', 'pageview', url);
        }
      });

      //this is for the google analytics to track site searches
      var path = window.location.pathname;
      if (path.indexOf("islandora/search") != -1) {
        //decode the url
        advancedPath = decodeURIComponent(path);
        //initialize the trackPageView
        var trackPageViewString = '/islandora/search/';

        //regular expression for advanced search
        var advancedRegex = new RegExp(/\((.*?)\)/);
        var result = advancedPath.match(advancedRegex);

        //if there is no result try the advanced regular expression
        if (!result ) {
          //regular expression for simple search
          var basicRegex =new RegExp("/islandora/search/([^/]*)/?(.*)?");
          //try and match the simple search
          var result = path.match(basicRegex);
        }

        //concatenate the search term to the trackpageview variable
        trackPageViewString = trackPageViewString.concat('?q=' + (result == null ? '' : result[1]));

        //we need to replace the " to be # for the regular expression to work
        var facetPath = decodeURIComponent(window.location.href);
        var facetPath = facetPath.replace(/"/g,'#');

        //regular expression to strip out the facets and push them in an array
        var facetRegEx = new RegExp(/\#(.*?)\#/g);
        var facetArray = facetPath.match(facetRegEx);

        //if there are contents in the array we want to stripe out the #
        if (facetArray) {
          for (var i = 0; i<facetArray.length;i++) {
            facetArray[i] = facetArray[i].replace(/#/g,'');
          }
        }

        //concatenate all of the facet terms to the trackpageview variable
        if (facetArray) {
          trackPageViewString = trackPageViewString.concat('&c='+facetArray[0]);
          for (var j = 1; j < facetArray.length;j++) {
            trackPageViewString = trackPageViewString.concat('&c='+facetArray[j]);
          }
        }

        if (typeof _gaq != null && typeof _gaq != "undefined") {
          _gaq.push(['_trackPageview', trackPageViewString]);
          _gaq.push(["_setAccount", ""]);
        }
        else if (typeof ga != null && typeof ga != "undefined") {
          ga('send', 'pageview', trackPageViewString);
        }
      }
    }
  };

})(jQuery);


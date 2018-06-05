SUMMARY
-------

This module provides a number of views that implement Google Analytics Views to pull statistical reports about content views and downloads.

A small snippet of javascript is also included to ensure Islandora datastream downloads are tracked by Google Ananlytics.


REQUIREMENTS
------------

 * Google Analytics Reports (http://www.drupal.org/project/google_analytics_reports)
 * Google Analytics Views (http://www.drupal.org/project/google_analytics_reports) - unreleased, in dev
 * Views


INSTALLATION
------------

Google Analytics tracking should be setup independent of this reporting aspect. A Google Analytics module is available for tracking
(http://www.drupal.org/project/google_analytics).


CONFIGURATION
-------------

Grant access to Google Analytics Reports to Google Analytics data at /admin/config/system/google-analytics-reports using the Google Analytics account
associated with the site's tracking setup.

For Site search and facets, click on the admin tab on the google analytics site.  Click on the profile link and go to the profile settings.
Check the "Do track Site Search" and enter q in the query parameter.  To enable the facets to be tracked click on the "Site search categories"
checkbox and enter c in the category paramneter.  Apply the changes. 


CUSTOMIZATION
-------------


TROUBLESHOOTING
---------------

Analytics data is cached because of quotas placed on the Google Analytics API. Clear cache to update statistics. Reporting data may be delayed by
several hours from the time of the tracking event.

F.A.Q.
------

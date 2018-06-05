# barnard_islandora_drush

This module contains definitions of custom [drush](http://www.drush.org/en/master/) commands used in administering [Barnard's Digital Collections](http://digitalcollections.barnard.edu). Two are particularly useful:

* `purge_object` (`bcipo`): Purges an object from the repository. (e.g.: `drush -u 1 purge_object bc:yearbook-1985`)
* `ingest_objects` (`bciio`): Ingests objects. `drush help bciio` yields many examples.

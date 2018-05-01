BUILD STATUS
------------
Current build status:
[![Build Status](https://travis-ci.org/discoverygarden/islandora_jodconverter.png?branch=7.x)](https://travis-ci.org/discoverygarden/islandora_jodconverter)

CI Server:
http://jenkins.discoverygarden.ca

SUMMARY
-------
Utilizes OpenOffice / JODCoverter as a service to convert documents between various formats.

### Supported formats

__PDF format__
* Portable Document Format (*.pdf)

__Text Formats__
* OpenDocument Text (*.odt)
* OpenOffice.org 1.0 Text (*.sxw)
* Rich Text Format (*.rtf)
* Microsoft Word (*.doc)
* WordPerfect (*.wpd)
* Plain Text (*.txt)
* HTML1 (*.html)

__Spreadsheet Formats__
* OpenDocument Spreadsheet (*.ods)
* OpenOffice.org 1.0 Spreadsheet (*.sxc)
* Microsoft Excel (*.xls)
* Comma-Separated Values (*.csv)
* Tab-Separated Values (*.tsv)

__Presentation Formats__
* OpenDocument Presentation (*.odp)
* OpenOffice.org 1.0 Presentation (*.sxi)
* Microsoft PowerPoint (*.ppt)


REQUIREMENTS
------------
* [Open Office](https://www.openoffice.org/)
* [JODConverter] (http://sourceforge.net/projects/jodconverter/)
* [libraries](https://drupal.org/project/libraries)

INSTALLATION
------------
### Open Office (Under Ubuntu 13.04)

Install Open Office and the required dependencies.
```sh
sudo add-apt-repository ppa:upubuntu-com/office
sudo apt-get update
sudo apt-get install openoffice
sudo apt-get install openoffice.org-writer
sudo apt-get install openoffice.org-draw
sudo apt-get install openoffice.org-calc
sudo apt-get install openoffice.org-impress
```

Start OpenOffice service:

Make sure you start OpenOffice as the same user that apache is running as.
```sh
soffice --headless --accept=socket,host=127.0.0.1,port=8100;urp;
```
or
```sh
soffice -headless -accept="socket,host=127.0.0.1,port=8100;urp;" -nofirststartwizard
```
If you are using an older version

### JODConverter
Download from [here](http://sourceforge.net/projects/jodconverter/files/JODConverter/2.2.2/jodconverter-2.2.2.zip/download)
Unzip it and place into Drupal/base/path/sites/all/libraries

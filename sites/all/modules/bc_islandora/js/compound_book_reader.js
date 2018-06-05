/**
 * seeks to extend the BookReader prototype to become aware of Islandora
 * Compound Objects.
 *
 * Islandora Compound Objects are a RDFS that describes an object's
 * relationship to any items included on/with that object at a given
 * sequence/position, their relation to one another, and their relation back to
 * the parent.
 *
 * e.g.: a playbill with a ticket stub located on page @ position 5, a
 * scrapbook with ephemera on several pages, sometimes more than one per page,
 * etc...
 *
 * @author Benjamin Rosner (github: br2490) - 7/17/2017 first commit.
 *
 * @licence adopts most restrictive of Islandora Internet Archive BookReader or
 *     Open Library Internet Archive BookReader
 * @see: https://github.com/internetarchive/bookreader (1.x)
 * @see: https://github.com/Islandora/internet_archive_bookreader (7.x-1.x)
 *
 * @todo
 */
(function ($) {
  // SMH.
  BookReader.prototype._init = BookReader.prototype.init;
  BookReader.prototype.init = function () {
    // This is first so we can set the initial display mode for compound
    // objects. Otherwise you can skip it.
    this.getCompoundInformation();

    this.bookReaderInitialMode = this.settings.bookReaderInitialMode || 0;
    if (this.bookReaderInitialMode) {
      this.settings.mode = this.bookReaderInitialMode;
//    console.log(this);
      this.initialDisplayOverride();
    }

    this._init.apply(this);

    this.updateToolbarDisplay();
  };

  /**
   * Put handlers here for when we will navigate to a new page.
   *
   * This has been extended for Compound Objects.
   */
  BookReader.prototype.willChangeToIndex = function (index) {
    // Update navbar position icon - leads page change animation
    this.updateNavIndex(index);

    if (this.bookReaderOnCompound) {
      this.updateCompoundBlockContent(index);
    }
  };

  /**
   * Returns true if we can switch to the requested mode
   *
   * This has been extended for Compound Objects.
   */
  BookReader.prototype.canSwitchToMode = function (mode) {
    if (this.bookReaderOnCompound) {
      // compound objects are kept within 1up and thumbs! and only some
      // scrapbooks are allowed thumbs!
      return (mode === this.constMode1up || mode === this.bookReaderInitialMode);
    }

    if (mode === this.constMode2up || mode === this.constModeThumb) {
      // check there are enough pages to display
      // $$$ this is a workaround for the mis-feature that we can't display
      // short books in 2up mode
      if (this.numLeafs < 2) {
        return false;
      }
    }
    return true;
  };

  /**
   * Determines all information about a Compound Object.
   *
   * BookReader object additional properties:
   * - compoundBlock: jQuery object of the Compound Block.
   * - compoundInclusionObjects: jQuery object of all compound objects in the
   * Compound Block.
   * - bookReaderOnCompound: true|false
   * - bookReaderOnInclusion: true|false - Are we on an active inclusion?
   * (i.e., not the parent, but a child object.)
   * - compoundInclusionPageNum: inclusion page number. (not the same as book
   * location/page number.)
   * - compoundReturnUrl: return link to parent object on last page viewed.
   *
   * @returns {boolean} true if this is a compound. otherwise false.
   */
  BookReader.prototype.getCompoundInformation = function () {
    // This will serve as a context (if it exists) save it.
    this.compoundBlock = $('#block-islandora-compound-object-compound-navigation');
    // This will serve as a context.  save it.
    this.compoundInclusionObjects = this.compoundBlock.find(".islandora-compound-thumb");

    // We should already know this is true, but in case the block is preset but
    // no objects are included.
    this.bookReaderOnCompound = this.compoundInclusionObjects.length > 0;
    if (!this.bookReaderOnCompound) {
      return false;
    }
    this.bookReaderInitialMode = 1;

    // BARNARD: We do not want to display the parent object.
    this.compoundBlock.find(".parent").hide();

    // Are we on an active inclusion? (i.e., not the parent, but a child
    // object.)
    var compoundActiveInclusionObjects = this.compoundInclusionObjects.has(".active .inclusion-object");
    this.bookReaderOnInclusion = compoundActiveInclusionObjects.length > 0;

    if (this.bookReaderOnInclusion) {
      // Pull our inclusion page identifier from the classes that wrap.
      this.compoundInclusionPageNum = compoundActiveInclusionObjects["0"].classList[3].match(/inclusion-page-(.*)/i)[1];
      // This is a context used in the updateCompoundBlockContent.
      this.compoundActiveInclusion = $(".inclusion-page-" + this.compoundInclusionPageNum);

      // Why am I relying on the breadcrumbs?  Because we can?  Sanity check,
      // please.
      var parentAlumBreadcrumb = $("div.breadcrumb a:last");
      this.compoundReturnUrl = parentAlumBreadcrumb.attr('href') + '#page/' + this.compoundInclusionPageNum;
      parentAlumBreadcrumb.attr('href', this.compoundReturnUrl);
      this.compoundInclusionObjects.find("a.active").attr('href', this.compoundReturnUrl);
    }

    this.updateCompoundBlockContent(this.currentIndex());
    this.updateCompoundBlockLocation();

    return true;
  };

  /**
   * Method to change the hash location if we're overriding the initially set
   * display mode.
   *
   * This happens before final init of the BookReader, so that
   * paramsFromFragment() can consume our hash and do the rest for us.
   */
  BookReader.prototype.initialDisplayOverride = function () {
    var hash = window.location.hash,
        mode = (this.bookReaderInitialMode > 0 && this.bookReaderInitialMode < 3 ? this.bookReaderInitialMode + 'up' : 'thumb' ),
        re = new RegExp(/mode\/(1up|2up|thumb)/, 'g');

    if (hash.substr(0, 1) !== '#') {
      window.location.hash = 'mode/' + mode;
    }
    else {
      hash = hash.substr(1);
      window.location.hash = hash.replace(re, 'mode/' + mode);
    }

  };

  /**
   * Actually update the jToolbar (BookReader calls it during init).
   *
   * Hide unavailable mode icons, resize the controls accordingly.
   */
  BookReader.prototype.updateToolbarDisplay = function () {
    var
        navigationButtons = $('#BRpage'),
        navigationSize = navigationButtons.width(),
        iconSize = $('.BRicon').width();

    // Why am I hanging on to this?
    this.availableModes = {
      'onePg': this.canSwitchToMode(1),
      'twoPg': this.canSwitchToMode(2),
      'thumb': this.canSwitchToMode(3),
    };

    for (var mode in this.availableModes) {
      if (this.availableModes[mode] !== true) {
        $('.' + mode.toLowerCase()).hide();
        navigationSize -= iconSize;
      }
    }

    $("#BRnavpos").css({'margin-right': navigationSize});
    navigationButtons.css({'width': navigationSize});
  };

  /**
   * Realign the Compound Block to the top of the BookReader object.
   */
  BookReader.prototype.updateCompoundBlockLocation = function () {
    // Block reposition
    var
        thumbContainer = $('.islandora-compound-thumbs'),
        blockContent = $('.islandora-compound-prev-next'),
        bTop = blockContent.offset().top,
        rTop = $('#BookReader').offset().top;

    blockContent
        .offset({
          'left': this.compoundBlock.offset().left,
        })
        .css({
          'margin-top': rTop - bTop,
        });

    thumbContainer
        .css({
          'max-height': '635px',
        });

  };

  /**
   * Fires on page change to hide and show the appropriate inclusion objects,
   * related titles, links, etc.
   *
   * This just sets a title and shows/hides inclusions as you navigate the
   * book.
   *
   * @param index
   */
  BookReader.prototype.updateCompoundBlockContent = function (index) {
    var pageInclusions = this.compoundActiveInclusion || $('.inclusion-page-' + this.getPageNum(index));
    // Show previously hidden inclusion.
    pageInclusions.has(":hidden").show();
    // Hide all inclusions NOT the current inclusion objects.
    this.compoundInclusionObjects.not(pageInclusions).hide();

    // Set a heading.
    if (this.bookReaderOnInclusion) {
      $("#return-to-page").html(Drupal.t('<a href="@url">Return to scrapbook page @page</a>', {
        '@url': this.compoundReturnUrl,
        '@page': this.compoundInclusionPageNum
      }));
      $(".islandora-compound-details").html(Drupal.t('Viewing @count @inclusion from <a href="@url">scrapbook page @page</a>', {
        '@url': this.compoundReturnUrl,
        '@count': pageInclusions.length / 2,
        '@inclusion': Drupal.formatPlural(pageInclusions.length / 2, 'inclusion', 'inclusions'),
        '@page': this.compoundInclusionPageNum
      }));
    }
    else {
      if (pageInclusions.length > 0) {
        $(".islandora-compound-details").text(Drupal.t('Displaying @count @inclusion from page @page', {
          '@count': pageInclusions.length / 2,
          '@inclusion': Drupal.formatPlural(pageInclusions.length, 'inclusion', 'inclusions'),
          '@page': this.getPageNum(index)
        }));
      }
      else {
        $(".islandora-compound-details").text(Drupal.t('No inclusion to display from page @page', {
          '@page': this.getPageNum(index)
        }));
      }
    }
  };
})(jQuery);

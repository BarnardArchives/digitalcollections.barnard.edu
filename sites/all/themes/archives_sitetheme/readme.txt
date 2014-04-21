Creating the new theme: 

  - Decide on the microsite theme starterkit the new microsite should use. (Either "microsite" or 
    "microsite_2")   - Copy the entire directory and contents of "example_sitetheme" found at sites/themes/
    example_sitetheme and rename to  "newsitename_sitetheme". For example "amec_sitetheme".
  - Inside of the css folder there is two site-color.css files. One for the "microsite" theme and one for 
    "microsite_2". Change the name of the file that this theme will use to site-colors.css. This will become the 
    css file to change the color scheme of the new theme. 
  - Change the name of example_sitetheme.info to the name of the site + _sitetheme.info. For example 
    "amec_sitetheme.info".
  - Edit the .info file. 
  - Change the name from "Example Site Theme" to "Sitename" Site Theme. 
  - Change the base theme by commenting out the base theme that is not in use and uncommenting out the 
     base theme that will be used. To comment add ";" to the beginning of the line. To uncomment simply 
     remove the semi colon.  


Enable the theme: 

  - In your Drupal site, navigate to admin/config/development/performance. 
  - In order for the new theme to be able to be used on the site the theme registry needs to be rebuilt by 
    clearing all caches. To do this click on the "clear all caches button". Note Clearing all caches on a live 
    site will momentarily effect the performance of the site. 
  - Next, navigate to admin/appearance 
  - Find your theme on this page and enable it.  


Before Creating the domain: 

  - Add some new content, especially the node you want to use for the home page. Make note of this node's 
    path, you will need to input it in the domain settings. 
  - Build your new site's main menu   


Creating the new domain: 

  - Go to admin/structure/domain   - Add a new domain.
  - Enter the domain, site name, url scheme and domain status. 
  - Edit the settings for the new domain.
  - Associate your new domain with the menu you just created. 
  - Edit the theme settings for the new domain and select the new microsite theme as the default for that  
     domain.
  - From within the theme settings for the new domain, be sure to uncheck "main menu" and "secondary 
    menu" as they are not needed.
  - After you have the domain setup you can affiliate the content you added with the new domain and you 
    can add a "menu_block" for your site's menu, then place it in the new theme.  
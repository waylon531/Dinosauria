Glossy Serpent is a free theme for CEGUI, based on the Taharez Look theme.
Glossy Serpent is licensed under the terms of the MIT license.
Please consult the license file distributed with this skin for a full description.

Current version: 1.0
Author: Augustin Preda


File list:
~~~~~~~~~~
(X and Y are considered the major and minor revision numbers)

- GlossySerpent.X.Y.sample.win32.7z
      A binary win32 sample application. Uses OGRE.
- GlossySerpent.X.Y.7z
      The default release of the Glossy Serpent skin. Contains all the files needed to add the skin to your application.
- GlossySerpent.X.Y.ceed.7z
      (DISCONTINUED)
      A customized release of the Glossy Serpent skin. Was used previously to apply minor fixes to the skin in order
      to make it compatible with previous versions of CEED (Crazy Eddie's Editor). Some features that worked in the code
      were not yet supported by CEED. The dedicated version simply alleviated the problems.
      

Note:
~~~~~
Additional files (E.g. GIMP source files) can be found on the project's SVN repository.

Version history
~~~~~~~~~~~~~~~
Version 1.1
~~~~~~~~~~~
Removed some un-needed meshes from the sample. Minor code cleanups
Added new widget: SimpleBorderButton.
Added new widget: SerpentScaledTopBorder
Added new widget: SerpentScaledBottomBorde
Added new widget: RailedGlyphButton (used only in FrameWindow)

Modified FrameWindow and SerpentPanel: these widgets now contain a subtle highlight of scales.
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
- Added new property to the FrameWindow: TitlebarSubPatternTexture, to draw a tiled texture over the titlebar.
(default value: GlossySerpentFHD/SerpentSkinTileDark).
To get the old look and feel of a window, please specify the following property in the layout file:
<Window type="GlossySerpentFHD/FrameWindow" name="yourName" >
  <Property name="TitlebarSubPatternTexture" value="" />

- Added new property to the FrameWindow and SerpentPanel: BackgroundScalesPictureColour, default value: "4714120F"
To get the old look and feel of a window, please specify the following property in the layout file:
<Window type="GlossySerpentFHD/FrameWindow" name="yourName" >
  <Property name="BackgroundScalesPictureColour" value="00FFFFFF" />

The highlight is based on a scale armour texture from here: http://opengameart.org/content/commission-medieval

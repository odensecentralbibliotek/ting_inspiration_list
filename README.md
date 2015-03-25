ting_inspiration_pages
======================

Description
-----------
This module defines a new page type: Inspiration list. In order to create a Inspiration page you define a query against the datawell.
When the inspiration page is displayed the module retrieves a large amount of results from the datawell and sorts them according to how
many copies of a material the library has. Thereafter the module checks if there are covers for the materials and shows a list of covers on
the page.

Because each display of a Inspiration page requires multiple calls to the datawell the results from the datawell are cached up to 14 days. Each night
there is a cronjob which refreshes a portion of the oldest pages in the cache.

Installation
-----------

Enable the Ting Inspiration List feature

Configuration
-------------

Under admin/config/ting/inspiration-list you can define following settings:

Cache lifetime:   The number of days each inspiration page datawell results are stored in cache.

Cache amount      The number of pages to refresh each night. 

Refreshing the cache of every page each night is performance costly and unnecessary. You should use settings which refreshes 
every page evenly through the Cache lifetime period. The config page also has a list of every Inspiration List page and the last
cache time. 




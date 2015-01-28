<?php
/**
 * @file
 * Default theme implementation for displaying ting new materials results.
 *
 */
?>
<div class="ting-inspiration-list">
  <?php if ($title) : ?>
    <div class="ting-inspiration-header">
      <div class="ting-inspiration-title">
        <h1><?php print $title; ?></h1>
      </div>
    </div>
  <?php endif; ?>
  <?php if ($results) : ?>
    <?php print drupal_render($results); ?>
  <?php else : ?>
    <div class="no-results-this-period">
      <?php print t('Your search yielded no results'); ?>
    </div>
  <?php endif; ?>
</div>

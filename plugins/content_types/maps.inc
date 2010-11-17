<?php
// $Id$

/**
 * Callback function to supply a list of content types.
 *
 * This is only required to make CTools know that we're providing 
 * multiple types here (since we have no single => TRUE in the array).
 */
function panels_gmap_maps_ctools_content_types() {
  return array(
    'title' => t('Google maps'),
  );
}

/**
 * Callback function to supply a list of content types.
 */
function panels_gmap_maps_content_type_content_types() {
  $types = array(
    'maps' => array(
      'title' => t('Google Map (dynamic)'),
      'description' => t('Displays a Google Map.'),
      'category' => t('Location'),
      'defaults' => array(
         'map_width' => '300',
         'map_height' => '250',
         'map_zoom' => '14',
      ),
      'required context' => new ctools_context_required(t('Node'), 'node'),
    ),
  );
  
  return $types;
}

/**
 * Return the pane contents.
 */
function panels_gmap_maps_content_type_render($subtype, $conf, $panel_args, $context) {
  $block = new stdClass();
  $node = isset($context->data) ? drupal_clone($context->data) : NULL;
  switch($subtype) {
    case 'maps':
      $block->content = theme('panels_google_maps', $node,
                              $conf['map_width'], $conf['map_height'],
                              $conf['map_zoom'], $conf['location_field']);
      break;
  }
  return $block;
}

/**
 * Return the pane titles.
 */
function panels_gmap_maps_content_type_admin_title($subtype, $conf, $context) {
  switch($subtype) {
    case 'maps':
      return t('"@s":  Google maps', array('@s' => $context->identifier));
  }
}

/**
 * Returns an edit form for the content type.
 */
function panels_gmap_maps_content_type_edit_form(&$form, &$form_state) {
  $conf = $form_state['conf'];
  switch ($form_state['subtype_name']) {
    case 'maps':
      $form['maps'] = array(
        '#type' => 'fieldset',
        '#title' => check_plain('Google maps settings'),
        '#collapsible' => TRUE,
        '#collapsed' => FALSE,
      );

      $form['maps']['map_width'] = array(
        '#type' => 'textfield',
        '#title' => t('Map width in pixel'),
        '#default_value' => $conf['map_width'],
      );

      $form['maps']['map_height'] = array(
        '#type' => 'textfield',
        '#title' => t('Map height in pixel'),
        '#default_value' => $conf['map_height'],
      );

      $form['maps']['map_zoom'] = array(
        '#type' => 'textfield',
        '#title' => t('Map zoom level'),
        '#default_value' => $conf['map_zoom'],
      );

      $fields = content_fields();
      $refs = array();
      foreach ($fields as $field) {
        if ($field['type'] == 'location') {
          $refs[$field['field_name']] = $field['field_name'];
        }
      }
      $form['maps']['location_field'] = array(
        '#type' => 'select',
        '#title' => t('CCK location field'),
        '#options' => $refs,
        '#default_value' => $conf['location_field'],
      );
      break;
  }
}
/**
 * Submit handler for the content type editing form.
 */
function panels_gmap_maps_content_type_edit_form_submit(&$form, &$form_state) {
  switch ($form_state['subtype_name']) {
    case 'maps':
      foreach (array('map_height', 'map_width', 'map_zoom', 'location_field') as $key) {
        $form_state['conf'][$key] = $form_state['values'][$key];
      }
      break;
  }
}
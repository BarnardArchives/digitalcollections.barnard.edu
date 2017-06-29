<?php

/**
 * @file
 * Defines a class which provides GET/POST/PUT/DELETE functions.
 */

class IslandoraHttpRestTestCase extends IslandoraWebTestCase {


  /**
   * Creates an admin user and a connection to a fedora repository.
   *
   * @see IslandoraWebTestCase::setUp()
   */
  public function setUp() {
    $args = func_get_args();
    $args = (isset($args[0]) && is_array($args[0])) ? $args[0] : $args;
    parent::setUp($args);
    // Use a cookie file, so login sessions will survive, curl_init().
    $this->cookieFile = drupal_tempnam(variable_get('file_temporary_path'), 'services_cookiejar');
    // Load the cookie file when initializing Curl.
    $this->additionalCurlOptions[CURLOPT_COOKIEFILE] = $this->cookieFile;
  }

  /**
   * Retrieves a Drupal path or an absolute path.
   *
   * @param string $path
   *   Drupal path or URL to load into internal browser
   * @param array $options
   *   Options to be forwarded to url().
   * @param array $headers
   *   An array containing additional HTTP request headers, each formatted as
   *   "name: value".
   *
   * @return array
   *   The retrieved JSON or plain string.
   */
  protected function httpGet($path, array $options = array(), array $headers = array()) {
    $response = $this->drupalGet($path, $options, $headers);
    switch ($this->drupalGetHeader('Content-Type')) {
      case 'application/json; utf-8':
        return drupal_json_decode($response);

      default:
        return $response;
    }
  }

  /**
   * Retrieves a Drupal path or an absolute path.
   *
   * @param string $path
   *   Drupal path or URL to load into internal browser.
   * @param array $options
   *   Options to be forwarded to url().
   * @param array $headers
   *   An array containing additional HTTP request headers, each formatted as
   *   "name: value".
   *
   * @return array
   *   The retrieved JSON or plain string.
   */
  protected function httpPut($path, array $data, array $options = array(), array $headers = array()) {
    $options['absolute'] = TRUE;
    // Assume for now that all PUT requests are JSON only.
    $data = drupal_json_encode($data);
    $fh = fopen('php://memory', 'rw');
    fwrite($fh, $data);
    // @codingStandardsIgnoreStart
    rewind($fh);
    // @codingStandardsIgnoreEnd
    $out = $this->curlExec(array(
             CURLOPT_PUT => TRUE,
             CURLOPT_INFILE => $fh,
             CURLOPT_INFILESIZE => strlen($data),
             CURLOPT_URL => url($path, $options),
             CURLOPT_NOBODY => FALSE,
             CURLOPT_HTTPHEADER => $headers));
    fclose($fh);
    // Ensure that any changes to variables in the other thread are picked up.
    $this->refreshVariables();
    $this->clearRepositoryCache();
    $this->verbose('PUT request to: ' . $path . '<hr />Ending URL: ' . $this->getUrl() . '<hr />' . $out);
    // Assume there will be no content response or it will be JSON.
    return drupal_json_decode($out);
  }

  /**
   * Retrieves a Drupal path or an absolute path.
   *
   * @param string $path
   *   Drupal path or URL to load into internal browser
   * @param array $data
   *   An array repersenting a json object.
   * @param array $options
   *   Options to be forwarded to url().
   * @param array $headers
   *   An array containing additional HTTP request headers, each formatted as
   *   "name: value".
   *
   * @return string
   *   The retrieved JSON or plain string.
   */
  protected function httpPost($path, $data, $options = array(), array $headers = array()) {
    // PHP 5.5.0 requires use of CURLFile().
    if (version_compare(PHP_VERSION, '5.5.0', '>=') && isset($data['file'])) {
      $fixture_path = ltrim($data['file'], '@');
      $data['file'] = new CURLFile($fixture_path, 'application/xml');
    }
    $options['absolute'] = TRUE;
    // Always assume POST request return JSON.
    $headers[] = "Accept: application/json";
    $out = $this->curlExec(array(
             CURLOPT_CUSTOMREQUEST => 'POST',
             CURLOPT_POST => TRUE,
             CURLOPT_POSTFIELDS => $data,
             CURLOPT_URL => url($path, $options),
             CURLOPT_NOBODY => FALSE,
             CURLOPT_HTTPHEADER => $headers));
    // Ensure that any changes to variables in the other thread are picked up.
    $this->refreshVariables();
    $this->clearRepositoryCache();
    $this->verbose('POST request to: ' . $path . '<hr />Ending URL: ' . $this->getUrl() . '<hr />' . $out);
    // Assume there will be no content response or it will be JSON.
    return drupal_json_decode($out);
  }

  /**
   * Makes a DELETE request to the given Drupal path or an absolute path.
   *
   * @param string $path
   *   Drupal path or URL to make the request to.
   * @param array $options
   *   Options to be forwarded to url().
   * @param array $headers
   *   An array containing additional HTTP request headers, each formatted as
   *   "name: value".
   *
   * @return array
   *   The retrieved JSON or plain string.
   */
  protected function httpDelete($path, $options = array(), array $headers = array()) {
    $options['absolute'] = TRUE;
    $out = $this->curlExec(array(
             CURLOPT_CUSTOMREQUEST => 'DELETE',
             CURLOPT_URL => url($path, $options),
             CURLOPT_NOBODY => FALSE,
             CURLOPT_HTTPHEADER => $headers));
    curl_setopt($this->curlHandle, CURLOPT_CUSTOMREQUEST, NULL);
    // Ensure that any changes to variables in the other thread are picked up.
    $this->refreshVariables();
    $this->clearRepositoryCache();
    $this->verbose('Delete request to: ' . $path . '<hr />Ending URL: ' . $this->getUrl() . '<hr />' . $out);
    // Assume there will be no content response or it will be JSON.
    return drupal_json_decode($out);
  }

  /**
   * Initializes and executes a cURL request.
   *
   * @param array $curl_options
   *   An associative array of cURL options to set, where the keys are constants
   *   defined by the cURL library. For a list of valid options, see
   *   http://www.php.net/manual/function.curl-setopt.php
   * @param bool $redirect
   *   FALSE if this is an initial request, TRUE if this request is the result
   *   of a redirect.
   *
   * @return mixed
   *   The content returned from the call to curl_exec().
   *
   * @see curlInitialize()
   */
  protected function curlExec($curl_options, $redirect = FALSE) {
    // Don't reuse the CURL connection since some requests like PUT
    // corrupt the connection.
    $this->curlClose();
    return parent::curlExec($curl_options, $redirect);
  }

  /**
   * Clear the Tuque Repository Cache.
   *
   * Some requests will invalidate the Tuque Cache, such as PUT/POST/DELETE's.
   */
  protected function clearRepositoryCache() {
    // @todo Make the cache reset more generic incase other caches get used.
    SimpleCache::resetCache();
  }

}

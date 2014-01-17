<?php

class TingClientMultiRequestAdapter extends TingClientRequestAdapter {
  /**
   * @var TingClientLogger
   */
  protected $logger;

  function __construct($options = array()) {
    $this->logger = new TingClientVoidLogger();
  }

  public function setLogger(TingClientLogger $logger) {
    $this->logger = $logger;
  }

  public function execute(TingClientRequest $requestObject) {
    $requests = $requestObject->requests ;
    $soap_requests = array();
   
    foreach ($requests as $request) {
      
      $client = new NanoSOAPClient($request->getWsdlUrl());
      //Prepare the parameters for the SOAP request
      $request->getRequest();
      $soapParameters = $request->getParameters();
      // Separate the action from other parameters
      $soapAction = $soapParameters['action'];
      unset($soapParameters['action']);

      // We use JSON as the default outputType.
      if (!isset($soapParameters['outputType'])) {
        $soapParameters['outputType'] = 'json';
      }
      $soap_requests[] = $this->buildSoapRequest($soapAction, $soapParameters, $client);
    }
    try {
      try {
        $startTime = explode(' ', microtime());

        $response = curl_multi($soap_requests); 
        $stopTime = explode(' ', microtime());
        $time = floatval(($stopTime[1]+$stopTime[0]) - ($startTime[1]+$startTime[0]));

        $this->logger->log('Completed SOAP request ' . $soapAction . ' ' . $request->getWsdlUrl() . ' (' . round($time, 3) . 's). Request body: ' . $client->requestBodyString . ' Response: ' . $response);

        // If using JSON and DKABM, we help parse it.
        if ($soapParameters['outputType'] == 'json') {
          $result = array();
          foreach ($response as $res) {
            $result[] = json_decode($res);
          }
          return $result;
        }
        else {
          return $response;
        }
      } catch (NanoSOAPcURLException $e) {
        //Convert NanoSOAP exceptions to TingClientExceptions as callers
        //should not deal with protocol details
        throw new TingClientException($e->getMessage(), $e->getCode());
      }
    } catch (TingClientException $e) {
      $this->logger->log('Error handling SOAP request ' . $soapAction . ' ' . $request->getWsdlUrl() .': '. $e->getMessage());
      throw $e;
    }
  }
  
  private function make_curl_call($requests) {
    // Initialise and configure cURL.
    $response = array();

    foreach ($requests as $request) {
      $ch = curl_init();
      $curl_options = $request['options'];
      $curl_options[CURLOPT_URL] = $request['endpoint'];
      curl_setopt_array($ch, $curl_options);

      $response[] = curl_exec($ch);

      if ($response === FALSE) {
        throw new NanoSOAPcURLException(curl_error($ch));
      }
      curl_close($ch);
    }
    // Close the cURL instance before we return.
    return $response;
  }
  
    /**
   * Make a SOAP request.
   *
   * @param string $action
   *   The SOAP action to perform/call.
   * @param array $parameters
   *   The parameters to send with the SOAP request.
   * @return string
   *   The SOAP response.
   */
  function buildSoapRequest($action, $parameters = array(), $client) {
    // Set content type and send the SOAP action as a header.
    $headers = array(
      'Content-Type: text/xml',
      'SOAPAction: ' . $action,
    );

    // Make a DOM document from the envelope and get the Body tag so we
    // can add our request data to it.
    $client->doc = new DOMDocument;
    $client->doc->loadXML($client->generateSOAPenvelope());
    $body = $client->doc->getElementsByTagName('Body')->item(0);

    // Convert the parameters into XML elements and add them to the 
    // body. The root element of this structure will be the action.
    $elem = $client->convertParameter($action, $parameters);
    $body->appendChild($elem);

    // Render and store the final request string.
    $client->requestBodyString = $client->doc->saveXML();

    // Send the SOAP request to the server via CURL.
    return $this->buildCurlRequest($client->endpoint, 'POST', $client->requestBodyString, $headers);
  }
  
    /**
   * Make a cURL request.
   *
   * This is usually a SOAP request, but could ostensibly be used for 
   * other things.
   *
   * @param string $url
   *   The URL to send the request to.
   * @param string $method
   *   The HTTP method to use. One of "GET" or "POST".
   * @param string $body
   *   The request body, ie. the SOAP envelope for SOAP requests.
   * @param array $headers
   *   Array of headers to be sent with the request.
   * @return string
   *   The response for the server, or FALSE on failure.
   */
  function buildCurlRequest($url, $method = 'GET', $body = '', $headers = array()) {
    $curl_session = array();    
    $curl_session['endpoint'] = $url;
    // Array of cURL options. See the documentation for curl_setopt for 
    // details on what options are available.
    $agent = 'Mozilla/5.0 (Windows; U; Windows NT 5.1; en-US; rv:1.9.0.8) Gecko/2009032609 Firefox/3.0.8';
    $curl_options = array(
     // CURLOPT_URL => $url,
      CURLOPT_USERAGENT => $agent,
      CURLOPT_RETURNTRANSFER => TRUE,
      CURLOPT_PROXY => '172.18.0.40:8080' //Todo remove developercode
    );

    if ($method == 'POST') {
      $curl_options[CURLOPT_POST] = TRUE;

      if (!empty($body)) {
        $curl_options[CURLOPT_POSTFIELDS] = $body;
      }
    }
    if (!empty($headers)) {
      $curl_options[CURLOPT_HTTPHEADER] = $headers;
    }
   $curl_session['options'] = $curl_options;    
   return $curl_session;
  }
}

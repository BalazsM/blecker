// This file is autogenerated. DO NOT MODIFY!!!

// /functions.js
const char* const data_functions_js_path PROGMEM = "/functions.js";
const char data_functions_js[] PROGMEM = R"=====(
// Must be a flat json object
let boardData = {};
let ajax = {};
let timer;
let boardname;
let resetconfig = false;
let advancedNetwork = false;
let rebootCheck;
let BOARD_NAME = "blecker";
// AJAX service
ajax.x = function () {
if (typeof XMLHttpRequest !== 'undefined') {
return new XMLHttpRequest();
}
var versions = [
"MSXML2.XmlHttp.6.0",
"MSXML2.XmlHttp.5.0",
"MSXML2.XmlHttp.4.0",
"MSXML2.XmlHttp.3.0",
"MSXML2.XmlHttp.2.0",
"Microsoft.XmlHttp"
];
var xhr;
for (var i = 0; i < versions.length; i++) {
try {
xhr = new ActiveXObject(versions[i]);
break;
} catch (e) {
}
}
return xhr;
};
ajax.send = function (url, callback, method, data, async) {
if (async === undefined) {
async = true;
}
var x = ajax.x();
x.open(method, url, async);
x.onreadystatechange = function () {
if (x.readyState == 4) {
callback(x.responseText, x.status)
}
};
if (method == 'POST') {
x.setRequestHeader('Content-type', 'application/x-www-form-urlencoded');
}
x.send(data)
};
ajax.get = function (url, data, callback, async) {
var query = [];
for (var key in data) {
query.push(key + '=' + data[key]);
}
ajax.send(url + (query.length ? '?' + query.join('&') : ''), callback, 'GET', null, async)
};
ajax.post = function (url, data, callback, async) {
var query = [];
for (var key in data) {
query.push(key + '=' + data[key]);
}
ajax.send(url, callback, 'POST', query.join('&'), async)
};
// Ajax upload
// Function that will allow us to know if Ajax uploads are supported
function supportAjaxUploadWithProgress() {
return supportFileAPI() && supportAjaxUploadProgressEvents() && supportFormData();
// Is the File API supported?
function supportFileAPI() {
var fi = document.createElement('INPUT');
fi.type = 'file';
return 'files' in fi;
};
// Are progress events supported?
function supportAjaxUploadProgressEvents() {
var xhr = new XMLHttpRequest();
return !! (xhr && ('upload' in xhr) && ('onprogress' in xhr.upload));
};
// Is FormData supported?
function supportFormData() {
return !! window.FormData;
}
}
function initFullFormAjaxUpload() {
var form = document.getElementById('form-id');
form.onsubmit = function() {
// FormData receives the whole form
var formData = new FormData(form);
// We send the data where the form wanted
var action = form.getAttribute('action');
// Code common to both variants
sendXHRequest(formData, action);
// Avoid normal form submission
return false;
}
}
function initFileOnlyAjaxUpload() {
var uploadBtn = document.getElementById('upload-button-id');
uploadBtn.onclick = function (evt) {
var formData = new FormData();
// Since this is the file only, we send it to a specific location
var action = '/upgrade';
// FormData only has the file
var fileInput = document.getElementById('file-id');
var file = fileInput.files[0];
formData.append('our-file', file);
getItem("custom-file-upload").style.display = "none";
uploadBtn.style.display = "none";
// Code common to both variants
sendXHRequest(formData, action);
}
}
// Once the FormData instance is ready and we know
// where to send the data, the code is the same
// for both variants of this technique
function sendXHRequest(formData, uri) {
// Get an XMLHttpRequest instance
var xhr = new XMLHttpRequest();
// Set up events
xhr.upload.addEventListener('loadstart', onloadstartHandler, false);
xhr.upload.addEventListener('progress', onprogressHandler, false);
xhr.upload.addEventListener('load', onloadHandler, false);
xhr.addEventListener('readystatechange', onreadystatechangeHandler, false);
// Set up request
xhr.open('POST', uri, true);
// Fire!
xhr.send(formData);
}
// Handle the start of the transmission
function onloadstartHandler(evt) {
var div = document.getElementById('upload-status');
div.innerHTML = 'Upload started.';
var loader = getItem("loader");
loader.style.display = "block";
rebootCheck = setInterval(ping, 60000);
}
// Handle the end of the transmission
function onloadHandler(evt) {
var div = document.getElementById('upload-status');
// div.innerHTML += '<' + 'br>File uploaded. Waiting for response.';
}
// Handle the progress
function onprogressHandler(evt) {
var div = document.getElementById('progress');
var percent = evt.loaded/evt.total*100;
// div.innerHTML = 'Progress: ' + percent + '%';
}
// Handle the response from the server
function onreadystatechangeHandler(evt) {
var status, text, readyState;
try {
readyState = evt.target.readyState;
text = evt.target.responseText;
status = evt.target.status;
}
catch(e) {
return;
}
if (readyState == 4 && status == '200' && evt.target.responseText) {
var status = document.getElementById('upload-status');
status.innerHTML += '<' + 'br>Success!';
// var result = document.getElementById('result');
// result.innerHTML = '<p>The server saw it as:</p><pre>' + evt.target.responseText + '</pre>';
}
}
// Clock functions
function clearconfig() {
if (getItem("reset").checked) {
resetconfig = true;
getItem("resetbutton").style.display = 'block';
getItem("savebutton").style.display = 'none';
} else {
resetconfig = false;
getItem("resetbutton").style.display = 'none';
getItem("savebutton").style.display = 'block';
}
}
function advanced() {
if (!advancedNetwork) {
getItem("networkmore").style.display = 'block';
getItem("advancednet").style.display = 'none';
getItem("basicnet").style.display = 'block';
advancedNetwork = true;
} else {
getItem("networkmore").style.display = 'none';
getItem("advancednet").style.display = 'block';
getItem("basicnet").style.display = 'none';
advancedNetwork = false;
}
}
function save(){
if (resetconfig) {
boardData = {
name: boardname
}
} else {
var button = getItem("savebutton");
var loader = getItem("loader");
loader.style.display = "block";
button.style.display = "none";
clockdata = collectData();

clockdata.name = boardname;
}
var json = JSON.stringify(clockdata);
ajax.post("/savedata", {data : json}, function(){});
rebootCheck = setInterval(ping, 20000);
}
function ping(){
ajax.get("/", {}, function(response, status){
if(status == 200) {
clearInterval(rebootCheck);
var button = getItem("savebutton");
var loader = getItem("loader");
loader.style.display = "none";
if (button) {
button.style.display = "block";
}
// Reload the page
window.location.href = "/";
}
});
}
function validateInteger(input){
var value = input.value; 
if (value == parseInt(value) && !value.includes(" ") && !value.includes(".")) {
return;
} else {
input.value = "";
return;
}
}
function collectData (){
var form = document.getElementById('dataform');
const values = {};
const inputs = form.elements;
for (let i = 0; i < inputs.length; i++) {
if (inputs[i].name) {
values[inputs[i].name] = encodeURIComponent(inputs[i].value);
}
}
return values;
}

function fillData() {
Object.entries(boardData).forEach(([key, value]) => {
if (getItem(key)) {
getItem(key).value = decodeURIComponent(value);
}
});
}

function pad(n) { return ("0" + n).slice(-2); }
function getItem(id) {
return document.getElementById(id);
}
function getData() {
ajax.get('/data', {}, function(response) {
//var response = '{"ssid":"","pw":"","timestamp":"1611008990","ntp":"","timezone":"","dst":false,"name":"24H"}';
if (response) {
boardData = JSON.parse(response);
boardname = boardData.name;
fillData();
} else {
console.log("Response was empty.");
}
});
}
// Add css slowly to mitigate the network traffic for ESP32
function loadCSS(cssName){
document.getElementsByTagName("head")[0].insertAdjacentHTML(
"beforeend",
"<link rel=\"stylesheet\" href=\"" + cssName + "\" />");
}
// Modal
var modal = document.getElementById("myModal");
var btn = document.getElementById("reset");
var span = document.getElementsByClassName("close")[0];
if (modal && btn && span) {
btn.onclick = function() {
modal.style.display = "block";
}
span.onclick = function() {
modal.style.display = "none";
}
window.onclick = function(event) {
if (event.target == modal) {
modal.style.display = "none";
}
}
}
setTimeout(function() {
if (location.pathname == "/" || location.pathname.includes("features")) {
getData();
}
}, 300);
setTimeout(function() {
loadCSS('/normalize.css');
}, 600);
setTimeout(function() {
loadCSS('/skeleton.css');
}, 900);
setTimeout(function() {
loadCSS('/style.css');
}, 1200);
 // Actually confirm support
 // initialize the ajax upload in the update page
 if (window.location.href.includes("update") && supportAjaxUploadWithProgress()) {
// Ajax uploads are supported!
// Change the support message and enable the upload button
var notice = document.getElementById('support-notice');

// Init the Ajax form submission
initFullFormAjaxUpload();
// Init the single-field file upload
initFileOnlyAjaxUpload();
}
function getFile() {
getItem("file-id").click();
}
function getFileName(obj) {
var file = obj.value;
var fileName = file.split("\\");
fileName = fileName[fileName.length - 1];
document.getElementById("fileinput").innerHTML = fileName;
var extension = fileName.substr(fileName.length - 4);
var uploadBtn = document.getElementById('upload-button-id');
var error = document.getElementById('error');
// Small verification
// file name should contains the BOARD_NAME string and the extension should be ".bin"
if (fileName.includes(BOARD_NAME + "_") && extension == ".bin") {
uploadBtn.removeAttribute('disabled');
error.style.display = "none";
} else {
error.style.display = "block";
}
}
)=====";

// /index.html
const char* const data_index_html_path PROGMEM = "/index.html";
const char data_index_html[] PROGMEM = R"=====(
<!DOCTYPE HTML><html>
<head>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
<meta name="viewport" content="width=device-width, initial-scale=1" />
<meta http-equiv="Cache-Control" content="no-cache, no-store, must-revalidate" />
<meta http-equiv="Pragma" content="no-cache" />
<meta http-equiv="Expires" content="0" />
<title>Blecker Administration</title>
</head>
<body>
<div id="loadingStart">Loading...</div>
<div id="main" style="display: none;">
<div class="loading" id="loader" style="display: none;">
<div style="margin-left: -25px">Loading...</div>
<div>&#8230;</div>
</div>
<div class="logo">
BLEcker
</div>
<div id="header">
<div class="row">
<h5><strong>BLE</strong> <i>tracker</i></h5>
</div>
</div>
<div id="sidebar">
<a class="button w100" href="/">home</a>
<a class="button w100" href="/update">update</a>
<!--<a class="button w100 button-danger" id="reset" href="#">reset</a>-->
<div class="version" id="version">v1.07 - 88</div>
<div id="footer">
<div><a href="https://github.com/redakker/blecker" target="_blank">blecker</a></div>
</div>
</div>
<div id="content">
<div class="contaier">
<!-- The Modal -->
<div id="myModal" class="modal" style="display: none;">
<!-- Modal content -->
<div class="modal-content">
<span class="close">&times;</span>
<p>Some text in the Modal..</p>
</div>
</div>
<div class="row">
<form id="dataform">
<div class="row" style="position: relative">
<h5 class="grey hrborder">Network settings</h5>
<div class="extrafunc pull-right text-warning hand" id="advancednet" onclick="advanced()">advanced +</div>
<div class="extrafunc pull-right text-warning hand" id="basicnet" style="display: none" onclick="advanced()">basic -</div>
</div>
<div class="row">
<div class="six columns">
<label for="ssid">WiFi name</label>
<input type="text" class="u-full-width" name="ssid" id="ssid" placeholder="SSID">
</div>
<div class="six columns">
<label for="pw">Password</label>
<input type="password" class="u-full-width" name="pw" id="pw" placeholder="Password">
</div>
</div>
<hr />
<div class="row">
<div class="six columns">
<label for="mqttserver">MQTT server</label>
<input type="text" class="u-full-width" name="mqttserver" id="mqttserver" placeholder="MQTT server address">
</div>
<div class="three columns">
<label for="mqttport">MQTT port</label>
<input type="text" class="u-full-width" value="1883" name="mqttport" id="mqttport" placeholder="MQTT server mqttport">
</div>
<div class="three columns">
<label for="mqttprefix">Base topic</label>
<input type="text" class="u-full-width" name="mqttprefix" id="mqttprefix" placeholder="MQTT topic prefix">
</div>
</div>
<div class="row">
<div class="six columns">
<label for="mqttuser">Username</label>
<input type="text" class="u-full-width" name="mqttuser" id="mqttuser" placeholder="Username">
</div>
<div class="six columns">
<label for="mqttpw">Password</label>
<input type="password" class="u-full-width" name="mqttpw" id="mqttpw" placeholder="Password">
</div>
</div>
<div style="display: none" id="networkmore">
<div class="row">
<div class="six columns">
<label for="present">Presence string (avalilable)</label>
<input type="text" class="u-full-width" name="present" id="present" placeholder="present">
</div>
<div class="six columns">
<label for="notpresent">Presence string (not available)</label>
<input type="text" class="u-full-width" name="notpresent" id="notpresent" placeholder="not present">
</div>
</div>
</div>
<hr />
<div class="row">
<label for="devices">Observed devices</label>
<input type="text" class="u-full-width" name="devices" id="devices" placeholder="ex.: 317234b9d2d0;15172f81accc;d0e003795c50">
<div class="inputcomment">Mac of the BLE devices.</div>
</div>
<hr />
<div class="row">
<label for="hadisc">Home Assistant Autodiscovery</label>
<select class="u-full-width" name="hadisc" id="hadisc">
<option value="0">Do not send</option>
<option value="1">Send</option>
</select>
<div class="inputcomment">Send autodiscovery message for Home Assistant (<a href="https://www.home-assistant.io/docs/mqtt/discovery/" target="_blank">details</a>)</div>
</div>
<div class="row">
<label for="hadiscpref">Autodiscovery prefix</label>
<input type="text" class="u-full-width" name="hadiscpref" id="hadiscpref" placeholder="homeassistant">
<div class="inputcomment">Default for HA is homeassistant</div>
</div>
<hr />
<div class="row">
<label for="whp">Webhook</label>
<input type="text" class="u-full-width" name="webhook" id="webhook" placeholder="http://example.com/{device}/{presence}">
<div class="inputcomment">{device} and {presence} will be replaced with the actual device name and state</div>
</div>
<hr />
<div class="row">
<label for="reboot">Reboot after (hours)</label>
<input type="text" class="u-full-width" name="reboot" id="reboot" onkeyup="validateInteger(this)" placeholder="">
<div class="inputcomment"></div>
</div>
<div class="row">
<label for="detailed">Detailed report</label>
<select class="u-full-width" name="detailed" id="detailed">
<option value="0">Do not send</option>
<option value="1">Send</option>
</select>
<div class="inputcomment">Send detailed MQTT report about the devices.</div>
</div>
<div class="row" style="height: 30px;"></div>
<input class="button-primary" type="button" value="Submit" id="savebutton" onclick="save()">
</form>
</div>
</div>
</div>
<script type="text/javascript" src="/functions.js"></script>
</div>
</body>
</html>

)=====";

// /normalize.css
const char* const data_normalize_css_path PROGMEM = "/normalize.css";
const char data_normalize_css[] PROGMEM = R"=====(
/*! normalize.css v3.0.2 | MIT License | git.io/normalize */
/**
 * 1. Set default font family to sans-serif.
 * 2. Prevent iOS text size adjust after orientation change, without disabling
 *user zoom.
 */
html {
font-family: sans-serif; /* 1 */
-ms-text-size-adjust: 100%; /* 2 */
-webkit-text-size-adjust: 100%; /* 2 */
}
/**
 * Remove default margin.
 */
body {
margin: 0;
}
/* HTML5 display definitions
 ========================================================================== */
/**
 * Correct `block` display not defined for any HTML5 element in IE 8/9.
 * Correct `block` display not defined for `details` or `summary` in IE 10/11
 * and Firefox.
 * Correct `block` display not defined for `main` in IE 11.
 */
article,
aside,
details,
figcaption,
figure,
footer,
header,
hgroup,
main,
menu,
nav,
section,
summary {
display: block;
}
/**
 * 1. Correct `inline-block` display not defined in IE 8/9.
 * 2. Normalize vertical alignment of `progress` in Chrome, Firefox, and Opera.
 */
audio,
canvas,
progress,
video {
display: inline-block; /* 1 */
vertical-align: baseline; /* 2 */
}
/**
 * Prevent modern browsers from displaying `audio` without controls.
 * Remove excess height in iOS 5 devices.
 */
audio:not([controls]) {
display: none;
height: 0;
}
/**
 * Address `[hidden]` styling not present in IE 8/9/10.
 * Hide the `template` element in IE 8/9/11, Safari, and Firefox < 22.
 */
[hidden],
template {
display: none;
}
/* Links
 ========================================================================== */
/**
 * Remove the gray background color from active links in IE 10.
 */
a {
background-color: transparent;
}
/**
 * Improve readability when focused and also mouse hovered in all browsers.
 */
a:active,
a:hover {
outline: 0;
}
/* Text-level semantics
 ========================================================================== */
/**
 * Address styling not present in IE 8/9/10/11, Safari, and Chrome.
 */
abbr[title] {
border-bottom: 1px dotted;
}
/**
 * Address style set to `bolder` in Firefox 4+, Safari, and Chrome.
 */
b,
strong {
font-weight: bold;
}
/**
 * Address styling not present in Safari and Chrome.
 */
dfn {
font-style: italic;
}
/**
 * Address variable `h1` font-size and margin within `section` and `article`
 * contexts in Firefox 4+, Safari, and Chrome.
 */
h1 {
font-size: 2em;
margin: 0.67em 0;
}
/**
 * Address styling not present in IE 8/9.
 */
mark {
background: #ff0;
color: #000;
}
/**
 * Address inconsistent and variable font size in all browsers.
 */
small {
font-size: 80%;
}
/**
 * Prevent `sub` and `sup` affecting `line-height` in all browsers.
 */
sub,
sup {
font-size: 75%;
line-height: 0;
position: relative;
vertical-align: baseline;
}
sup {
top: -0.5em;
}
sub {
bottom: -0.25em;
}
/* Embedded content
 ========================================================================== */
/**
 * Remove border when inside `a` element in IE 8/9/10.
 */
img {
border: 0;
}
/**
 * Correct overflow not hidden in IE 9/10/11.
 */
svg:not(:root) {
overflow: hidden;
}
/* Grouping content
 ========================================================================== */
/**
 * Address margin not present in IE 8/9 and Safari.
 */
figure {
margin: 1em 40px;
}
/**
 * Address differences between Firefox and other browsers.
 */
hr {
-moz-box-sizing: content-box;
box-sizing: content-box;
height: 0;
}
/**
 * Contain overflow in all browsers.
 */
pre {
overflow: auto;
}
/**
 * Address odd `em`-unit font size rendering in all browsers.
 */
code,
kbd,
pre,
samp {
font-family: monospace, monospace;
font-size: 1em;
}
/* Forms
 ========================================================================== */
/**
 * Known limitation: by default, Chrome and Safari on OS X allow very limited
 * styling of `select`, unless a `border` property is set.
 */
/**
 * 1. Correct color not being inherited.
 *Known issue: affects color of disabled elements.
 * 2. Correct font properties not being inherited.
 * 3. Address margins set differently in Firefox 4+, Safari, and Chrome.
 */
button,
input,
optgroup,
select,
textarea {
color: inherit; /* 1 */
font: inherit; /* 2 */
margin: 0; /* 3 */
}
/**
 * Address `overflow` set to `hidden` in IE 8/9/10/11.
 */
button {
overflow: visible;
}
/**
 * Address inconsistent `text-transform` inheritance for `button` and `select`.
 * All other form control elements do not inherit `text-transform` values.
 * Correct `button` style inheritance in Firefox, IE 8/9/10/11, and Opera.
 * Correct `select` style inheritance in Firefox.
 */
button,
select {
text-transform: none;
}
/**
 * 1. Avoid the WebKit bug in Android 4.0.* where (2) destroys native `audio`
 *and `video` controls.
 * 2. Correct inability to style clickable `input` types in iOS.
 * 3. Improve usability and consistency of cursor style between image-type
 *`input` and others.
 */
button,
html input[type="button"], /* 1 */
input[type="reset"],
input[type="submit"] {
-webkit-appearance: button; /* 2 */
cursor: pointer; /* 3 */
}
/**
 * Re-set default cursor for disabled elements.
 */
button[disabled],
html input[disabled] {
cursor: default;
}
/**
 * Remove inner padding and border in Firefox 4+.
 */
button::-moz-focus-inner,
input::-moz-focus-inner {
border: 0;
padding: 0;
}
/**
 * Address Firefox 4+ setting `line-height` on `input` using `!important` in
 * the UA stylesheet.
 */
input {
line-height: normal;
}
/**
 * It's recommended that you don't attempt to style these elements.
 * Firefox's implementation doesn't respect box-sizing, padding, or width.
 *
 * 1. Address box sizing set to `content-box` in IE 8/9/10.
 * 2. Remove excess padding in IE 8/9/10.
 */
input[type="checkbox"],
input[type="radio"] {
box-sizing: border-box; /* 1 */
padding: 0; /* 2 */
}
/**
 * Fix the cursor style for Chrome's increment/decrement buttons. For certain
 * `font-size` values of the `input`, it causes the cursor style of the
 * decrement button to change from `default` to `text`.
 */
input[type="number"]::-webkit-inner-spin-button,
input[type="number"]::-webkit-outer-spin-button {
height: auto;
}
/**
 * 1. Address `appearance` set to `searchfield` in Safari and Chrome.
 * 2. Address `box-sizing` set to `border-box` in Safari and Chrome
 *(include `-moz` to future-proof).
 */
input[type="search"] {
-webkit-appearance: textfield; /* 1 */
-moz-box-sizing: content-box;
-webkit-box-sizing: content-box; /* 2 */
box-sizing: content-box;
}
/**
 * Remove inner padding and search cancel button in Safari and Chrome on OS X.
 * Safari (but not Chrome) clips the cancel button when the search input has
 * padding (and `textfield` appearance).
 */
input[type="search"]::-webkit-search-cancel-button,
input[type="search"]::-webkit-search-decoration {
-webkit-appearance: none;
}
/**
 * Define consistent border, margin, and padding.
 */
fieldset {
border: 1px solid #c0c0c0;
margin: 0 2px;
padding: 0.35em 0.625em 0.75em;
}
/**
 * 1. Correct `color` not being inherited in IE 8/9/10/11.
 * 2. Remove padding so people aren't caught out if they zero out fieldsets.
 */
legend {
border: 0; /* 1 */
padding: 0; /* 2 */
}
/**
 * Remove default vertical scrollbar in IE 8/9/10/11.
 */
textarea {
overflow: auto;
}
/**
 * Don't inherit the `font-weight` (applied by a rule above).
 * NOTE: the default cannot safely be changed in Chrome and Safari on OS X.
 */
optgroup {
font-weight: bold;
}
/* Tables
 ========================================================================== */
/**
 * Remove most spacing between table cells.
 */
table {
border-collapse: collapse;
border-spacing: 0;
}
td,
th {
padding: 0;
}
)=====";

// /skeleton.css
const char* const data_skeleton_css_path PROGMEM = "/skeleton.css";
const char data_skeleton_css[] PROGMEM = R"=====(
.container {
position: relative;
width: 100%;
max-width: 960px;
margin: 0 auto;
padding: 0 20px;
box-sizing: border-box; }
.column,
.columns {
width: 100%;
float: left;
box-sizing: border-box; }

@media (min-width: 400px) {
.container {
width: 85%;
padding: 0; }
}

@media (min-width: 550px) {
.container {
width: 80%; }
.column,
.columns {
margin-left: 4%; }
.column:first-child,
.columns:first-child {
margin-left: 0; }
.one.column,
.one.columns{ width: 4.66666666667%; }
.two.columns{ width: 13.3333333333%; }
.three.columns{ width: 22%;}
.four.columns { width: 30.6666666667%; }
.five.columns { width: 39.3333333333%; }
.six.columns{ width: 48%;}
.seven.columns{ width: 56.6666666667%; }
.eight.columns{ width: 65.3333333333%; }
.nine.columns { width: 74.0%;}
.ten.columns{ width: 82.6666666667%; }
.eleven.columns { width: 91.3333333333%; }
.twelve.columns { width: 100%; margin-left: 0; }
.one-third.column { width: 30.6666666667%; }
.two-thirds.column{ width: 65.3333333333%; }
.one-half.column{ width: 48%; }

.offset-by-one.column,
.offset-by-one.columns{ margin-left: 8.66666666667%; }
.offset-by-two.column,
.offset-by-two.columns{ margin-left: 17.3333333333%; }
.offset-by-three.column,
.offset-by-three.columns{ margin-left: 26%;}
.offset-by-four.column,
.offset-by-four.columns { margin-left: 34.6666666667%; }
.offset-by-five.column,
.offset-by-five.columns { margin-left: 43.3333333333%; }
.offset-by-six.column,
.offset-by-six.columns{ margin-left: 52%;}
.offset-by-seven.column,
.offset-by-seven.columns{ margin-left: 60.6666666667%; }
.offset-by-eight.column,
.offset-by-eight.columns{ margin-left: 69.3333333333%; }
.offset-by-nine.column,
.offset-by-nine.columns { margin-left: 78.0%;}
.offset-by-ten.column,
.offset-by-ten.columns{ margin-left: 86.6666666667%; }
.offset-by-eleven.column,
.offset-by-eleven.columns { margin-left: 95.3333333333%; }
.offset-by-one-third.column,
.offset-by-one-third.columns{ margin-left: 34.6666666667%; }
.offset-by-two-thirds.column,
.offset-by-two-thirds.columns { margin-left: 69.3333333333%; }
.offset-by-one-half.column,
.offset-by-one-half.columns { margin-left: 52%; }
}


html {
font-size: 62.5%; }
body {
font-size: 1.5em; 
line-height: 1.6;
font-weight: 400;
font-family: "Raleway", "HelveticaNeue", "Helvetica Neue", Helvetica, Arial, sans-serif;
color: #222; }

h1, h2, h3, h4, h5, h6 {
margin-top: 0;
margin-bottom: 2rem;
font-weight: 300; }
h1 { font-size: 4.0rem; line-height: 1.2;letter-spacing: -.1rem;}
h2 { font-size: 3.6rem; line-height: 1.25; letter-spacing: -.1rem; }
h3 { font-size: 3.0rem; line-height: 1.3;letter-spacing: -.1rem; }
h4 { font-size: 2.4rem; line-height: 1.35; letter-spacing: -.08rem; }
h5 { font-size: 1.8rem; line-height: 1.5;letter-spacing: -.05rem; }
h6 { font-size: 1.5rem; line-height: 1.6;letter-spacing: 0; }

@media (min-width: 550px) {
h1 { font-size: 5.0rem; }
h2 { font-size: 4.2rem; }
h3 { font-size: 3.6rem; }
h4 { font-size: 3.0rem; }
h5 { font-size: 2.4rem; }
h6 { font-size: 1.5rem; }
}
p {
margin-top: 0; }

a {
color: #1EAEDB; }
a:hover {
color: #0FA0CE; }

.button,
button,
input[type="submit"],
input[type="reset"],
input[type="button"] {
display: inline-block;
height: 38px;
padding: 0 5px;
color: #555;
text-align: center;
font-size: 11px;
font-weight: 600;
line-height: 38px;
letter-spacing: .1rem;
text-transform: uppercase;
text-decoration: none;
white-space: nowrap;
background-color: transparent;
border-radius: 4px;
border: 1px solid #bbb;
cursor: pointer;
box-sizing: border-box; }
.button:hover,
button:hover,
input[type="submit"]:hover,
input[type="reset"]:hover,
input[type="button"]:hover,
.button:focus,
button:focus,
input[type="submit"]:focus,
input[type="reset"]:focus,
input[type="button"]:focus {
color: #333;
border-color: #888;
outline: 0; }
.button.button-primary,
button.button-primary,
input[type="submit"].button-primary,
input[type="reset"].button-primary,
input[type="button"].button-primary {
color: #FFF;
background-color: #33C3F0;
border-color: #33C3F0; }
.button.button-primary:hover,
button.button-primary:hover,
input[type="submit"].button-primary:hover,
input[type="reset"].button-primary:hover,
input[type="button"].button-primary:hover,
.button.button-primary:focus,
button.button-primary:focus,
input[type="submit"].button-primary:focus,
input[type="reset"].button-primary:focus,
input[type="button"].button-primary:focus {
color: #FFF;
background-color: #1EAEDB;
border-color: #1EAEDB; }
.button.button-warning,
button.button-warning,
input[type="submit"].button-warning,
input[type="reset"].button-warning,
input[type="button"].button-warning {
color: #FFF;
background-color: #f09e33;
border-color: #f09e33; }
.button.button-warning:hover,
button.button-warning:hover,
input[type="submit"].button-warning:hover,
input[type="reset"].button-warning:hover,
input[type="button"].button-warning:hover,
.button.button-warning:focus,
button.button-warning:focus,
input[type="submit"].button-warning:focus,
input[type="reset"].button-warning:focus,
input[type="button"].button-warning:focus {
color: #FFF;
background-color: #f09e33;
border-color: #f09e33; }
.button.button-danger,
button.button-danger,
input[type="submit"].button-danger,
input[type="reset"].button-danger,
input[type="button"].button-danger {
color: #FFF;
background-color: #c74f4f;
border-color: #c74f4f; }
.button.button-danger:hover,
button.button-danger:hover,
input[type="submit"].button-danger:hover,
input[type="reset"].button-danger:hover,
input[type="button"].button-danger:hover,
.button.button-danger:focus,
button.button-danger:focus,
input[type="submit"].button-danger:focus,
input[type="reset"].button-danger:focus,
input[type="button"].button-danger:focus {
color: #FFF;
background-color: #c74f4f;
border-color: #c74f4f; }

input[type="email"],
input[type="number"],
input[type="search"],
input[type="text"],
input[type="tel"],
input[type="url"],
input[type="password"],
textarea,
select {
height: 38px;
padding: 6px 10px; 
background-color: #fff;
border: 1px solid #D1D1D1;
border-radius: 4px;
box-shadow: none;
box-sizing: border-box; }
input[type="email"],
input[type="number"],
input[type="search"],
input[type="text"],
input[type="tel"],
input[type="url"],
input[type="password"],
textarea {
-webkit-appearance: none;
 -moz-appearance: none;
appearance: none; }
textarea {
min-height: 65px;
padding-top: 6px;
padding-bottom: 6px; }
input[type="email"]:focus,
input[type="number"]:focus,
input[type="search"]:focus,
input[type="text"]:focus,
input[type="tel"]:focus,
input[type="url"]:focus,
input[type="password"]:focus,
textarea:focus,
select:focus {
border: 1px solid #33C3F0;
outline: 0; }
label,
legend {
display: block;
margin-bottom: .5rem;
font-weight: 600; }
fieldset {
padding: 0;
border-width: 0; }
input[type="checkbox"],
input[type="radio"] {
display: inline; }
label > .label-body {
display: inline-block;
margin-left: .5rem;
font-weight: normal; }

ul {
list-style: circle inside; }
ol {
list-style: decimal inside; }
ol, ul {
padding-left: 0;
margin-top: 0; }
ul ul,
ul ol,
ol ol,
ol ul {
margin: 1.5rem 0 1.5rem 3rem;
font-size: 90%; }
li {
margin-bottom: 1rem; }

code {
padding: .2rem .5rem;
margin: 0 .2rem;
font-size: 90%;
white-space: nowrap;
background: #F1F1F1;
border: 1px solid #E1E1E1;
border-radius: 4px; }
pre > code {
display: block;
padding: 1rem 1.5rem;
white-space: pre; }

th,
td {
padding: 12px 15px;
text-align: left;
border-bottom: 1px solid #E1E1E1; }
th:first-child,
td:first-child {
padding-left: 0; }
th:last-child,
td:last-child {
padding-right: 0; }

button,
.button {
margin-bottom: 1rem; }
input,
textarea,
select,
fieldset {
margin-bottom: 1.5rem; }
pre,
blockquote,
dl,
figure,
table,
p,
ul,
ol,
form {
margin-bottom: 2.5rem; }

.u-full-width {
width: 100%;
box-sizing: border-box; }
.u-max-full-width {
max-width: 100%;
box-sizing: border-box; }
.u-pull-right {
float: right; }
.u-pull-left {
float: left; }

hr {
margin-top: 3rem;
margin-bottom: 3.5rem;
border-width: 0;
border-top: 1px solid #E1E1E1; }


.container:after,
.row:after,
.u-cf {
content: "";
display: table;
clear: both; }



@media (min-width: 400px) {}

@media (min-width: 550px) {}

@media (min-width: 750px) {}

@media (min-width: 1000px) {}

@media (min-width: 1200px) {}

)=====";

// /style.css
const char* const data_style_css_path PROGMEM = "/style.css";
const char data_style_css[] PROGMEM = R"=====(
hr {
margin-top: 2rem;
margin-bottom: 2.5rem;
border-width: 0;
border-top: 1px solid #E1E1E1;
}
.logo {
margin-top: 20px;
margin-left: 40px;
position: fixed;
top: 0;
left: 0;
z-index: 999;
font-weight: bolder;
}
#header {
height: 40px;
width: 100%;
position: fixed;
top: 0;
right: 0;
padding-top: 5px;
background-color: #FbFbFb;
color:#888;
z-index: 998;
text-align: right;
}
#header h5 {
margin-right: 20px;
margin-top: 5px;
}
#footer {
height: 30px;
width: 100%;
position: fixed;
bottom: 10px;
left: 10px;
font-size: 11px;
color:#888;
}
.version {
text-align: center;
font-size: 11px;
color:#888;
}
#sidebar {
height: 100%;
width: 120px;
position: fixed;
top: 0;
left: 0;
padding: 60px 5px 0 5px;
border: 1px solid #ececec;
border-width: 0 1px 0 0;
z-index: 998;
background-color: #FbFbFb;
}
#content {
padding-left: 155px;
padding-top: 70px;
padding-right: 30px;
}
input[type="file"] {
display: none;
}
.custom-file-upload {
border: 1px solid #ccc;
display: inline-block;
padding: 6px 12px;
cursor: pointer;
border-radius: 4px;
height: 25px;
}
.extrafunc {
position: absolute;
top: 30px;
right: -10px;
}
.grey {
color: #888
}
.text-primary {
color: #1EAEDB;
}
.text-success {
color: #60a75e;
}
.text-warning {
color: #f09e33;
}
.text-danger {
color: #c74f4f;
}
.hrborder {
border: 1px solid #ececec;
border-width: 0 0 1px 0;
max-width: 80%;
}
input:disabled {
background-color: #dddddd;
}
.w100 {
width: 100%;
}
.clear {
clear: both;
}
.hand {
cursor: pointer;
}
.center{
margin-left: 50%;
}
.hidden {
display: none;
}
.inputcomment {
color: #6e6e6e;;
font-size: 12px;
margin-top: -8px;
padding-left: 5px;
}
/* Absolute Center Spinner */
.loading {
position: fixed;
z-index: 999;
height: 2em;
width: 2em;
overflow: visible;
margin: auto;
top: 0;
left: 0;
bottom: 0;
right: 0;
}
/* Transparent Overlay */
.loading:before {
content: '';
display: block;
position: fixed;
top: 0;
left: 0;
width: 100%;
height: 100%;
background-color: rgba(202,202,202,0.9);
}
.loading:not(:required):after {
content: '';
display: block;
font-size: 10px;
width: 1em;
height: 1em;
margin-top: -0.5em;
-webkit-animation: spinner 1500ms infinite linear;
-moz-animation: spinner 1500ms infinite linear;
-ms-animation: spinner 1500ms infinite linear;
-o-animation: spinner 1500ms infinite linear;
animation: spinner 1500ms infinite linear;
border-radius: 0.5em;
-webkit-box-shadow: rgba(0, 0, 0, 0.75) 1.5em 0 0 0, rgba(0, 0, 0, 0.75) 1.1em 1.1em 0 0, rgba(0, 0, 0, 0.75) 0 1.5em 0 0, rgba(0, 0, 0, 0.75) -1.1em 1.1em 0 0, rgba(0, 0, 0, 0.5) -1.5em 0 0 0, rgba(0, 0, 0, 0.5) -1.1em -1.1em 0 0, rgba(0, 0, 0, 0.75) 0 -1.5em 0 0, rgba(0, 0, 0, 0.75) 1.1em -1.1em 0 0;
box-shadow: rgba(0, 0, 0, 0.75) 1.5em 0 0 0, rgba(0, 0, 0, 0.75) 1.1em 1.1em 0 0, rgba(0, 0, 0, 0.75) 0 1.5em 0 0, rgba(0, 0, 0, 0.75) -1.1em 1.1em 0 0, rgba(0, 0, 0, 0.75) -1.5em 0 0 0, rgba(0, 0, 0, 0.75) -1.1em -1.1em 0 0, rgba(0, 0, 0, 0.75) 0 -1.5em 0 0, rgba(0, 0, 0, 0.75) 1.1em -1.1em 0 0;
}
/* Animation */
@-webkit-keyframes spinner {
0% {
-webkit-transform: rotate(0deg);
-moz-transform: rotate(0deg);
-ms-transform: rotate(0deg);
-o-transform: rotate(0deg);
transform: rotate(0deg);
}
100% {
-webkit-transform: rotate(360deg);
-moz-transform: rotate(360deg);
-ms-transform: rotate(360deg);
-o-transform: rotate(360deg);
transform: rotate(360deg);
}
}
@-moz-keyframes spinner {
0% {
-webkit-transform: rotate(0deg);
-moz-transform: rotate(0deg);
-ms-transform: rotate(0deg);
-o-transform: rotate(0deg);
transform: rotate(0deg);
}
100% {
-webkit-transform: rotate(360deg);
-moz-transform: rotate(360deg);
-ms-transform: rotate(360deg);
-o-transform: rotate(360deg);
transform: rotate(360deg);
}
}
@-o-keyframes spinner {
0% {
-webkit-transform: rotate(0deg);
-moz-transform: rotate(0deg);
-ms-transform: rotate(0deg);
-o-transform: rotate(0deg);
transform: rotate(0deg);
}
100% {
-webkit-transform: rotate(360deg);
-moz-transform: rotate(360deg);
-ms-transform: rotate(360deg);
-o-transform: rotate(360deg);
transform: rotate(360deg);
}
}
@keyframes spinner {
0% {
-webkit-transform: rotate(0deg);
-moz-transform: rotate(0deg);
-ms-transform: rotate(0deg);
-o-transform: rotate(0deg);
transform: rotate(0deg);
}
100% {
-webkit-transform: rotate(360deg);
-moz-transform: rotate(360deg);
-ms-transform: rotate(360deg);
-o-transform: rotate(360deg);
transform: rotate(360deg);
}
}
/* The Modal (background) */
.modal {
display: none; /* Hidden by default */
position: fixed; /* Stay in place */
z-index: 1; /* Sit on top */
padding-top: 100px; /* Location of the box */
left: 0;
top: 0;
width: 100%; /* Full width */
height: 100%; /* Full height */
overflow: auto; /* Enable scroll if needed */
background-color: rgb(202,202,202); /* Fallback color */
background-color: rgba(202,202,202,0.9); /* Black w/ opacity */
}
/* Modal Content */
.modal-content {
background-color: #fefefe;
margin-left: 150px;
margin-right: 30px;
padding: 20px;
border: 1px solid #888;
}
/* The Close Button */
.close {
color: #aaaaaa;
float: right;
font-size: 28px;
font-weight: bold;
}
.close:hover,
.close:focus {
color: #000;
text-decoration: none;
cursor: pointer;
}
/* Start loading trick */
#loadingStart {display: none !important;}
#main { display: block !important; }
)=====";

// /update.
const char* const data_update_html_path PROGMEM = "/update.";
const char data_update_html[] PROGMEM = R"=====(
<!DOCTYPE HTML><html>
<head>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
<meta name="viewport" content="width=device-width, initial-scale=1" />
<meta http-equiv="Cache-Control" content="no-cache, no-store, must-revalidate" />
<meta http-equiv="Pragma" content="no-cache" />
<meta http-equiv="Expires" content="0" />
<title>Blecker Administration</title>
</head>
<body>
<div id="loadingStart">Loading...</div>
<div id="main" style="display: none;">
<div class="loading" id="loader" style="display: none;">
<div style="margin-left: -25px">Loading...</div>
<div>&#8230;</div>
</div>
<div class="logo">
BLEcker
</div>
<div id="header">
<div class="row">
<h5><strong>BLE</strong> <i>tracker</i></h5>
</div>
</div>
<div id="sidebar">
<a class="button w100" href="/">home</a>
<a class="button w100" href="/update">update</a>
<!--<a class="button w100 button-danger" id="reset" href="#">reset</a>-->
<div class="version" id="version">v1.07 - 88</div>
<div id="footer">
<div><a href="https://github.com/redakker/blecker" target="_blank">blecker</a></div>
</div>
</div>
<div id="content">
<div class="contaier">
<!-- The Modal -->
<div id="myModal" class="modal" style="display: none;">
<!-- Modal content -->
<div class="modal-content">
<span class="close">&times;</span>
<p>Some text in the Modal..</p>
</div>
</div>
<p id="support-notice">Please browse your update file from your device.<br />
Check for newer update files on <a href="https://github.com/redakker/blecker" target="blank">https://github.com/redakker/blecker</a> website.</p>
<p class="text-danger" id="error" style="display: none;">The chosen file is probably not a valid update file.</p>
<form action="/upgrade" method="post" enctype="multipart/form-data" id="form-id">
<label for="file-upload" id="custom-file-upload" class="custom-file-upload" onclick="getFile()">
<input id="file-id" type="file" name="our-file" onchange="getFileName(this)"/>
<span id="fileinput" class="">Choose a file!</span>
</label>
<input type="button" value="Upload" id="upload-button-id" disabled="disabled" />
<p id="upload-status"></p>
<p id="progress"></p>
<pre id="result"></pre>
</form>
</div>
</div>
<script type="text/javascript" src="/functions.js"></script>
</div>
</body>
</html>

)=====";


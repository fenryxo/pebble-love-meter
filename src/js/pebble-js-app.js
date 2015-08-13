var LoveMeter = {};

LoveMeter.onReady = function(e)
{
    Pebble.addEventListener('showConfiguration', this.onShowConfiguration.bind(this));
    Pebble.addEventListener('webviewclosed', this.onWebViewClosed.bind(this));
    console.log("Hello");
}

LoveMeter.onShowConfiguration = function(e)
{
  Pebble.openURL('http://localhost:1080/html/config-page.html');
}

LoveMeter.onWebViewClosed = function(e)
{
    var config = JSON.parse(decodeURIComponent(e.response));
    console.log(config);
}

Pebble.addEventListener('ready', LoveMeter.onReady.bind(LoveMeter));


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
    var message = {
        CONFIG_NAME: config["name"],
        CONFIG_RESULT: config["result"],
        CONFIG_DURATION: config["duration"]
    }
    var transactionId = Pebble.sendAppMessage(message,
        function(e)
        {
            console.log(
                'Successfully delivered message with transactionId=' + e.data.transactionId);
        },
        function(e)
        {
            console.log(
            'Unable to deliver message with transactionId='
            + e.data.transactionId + ' Error is: ' + e);
        }
    );
}

Pebble.addEventListener('ready', LoveMeter.onReady.bind(LoveMeter));


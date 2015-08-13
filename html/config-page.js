var LoveMeter = {};

LoveMeter.init = function()
{
    this.settings = {};
    this.keys = ["name", "duration", "result"];
    this.inputs = {};
    for (var i = 0; i < this.keys.length; i++)
    {
        var key = this.keys[i];
        this.inputs[key] = document.getElementById(key);
    }
    this.loadData();
    
    var submitButton = document.getElementById('submit_button');
    submitButton.addEventListener('click', LoveMeter.submit.bind(LoveMeter));
}

LoveMeter.loadData = function()
{
    if (localStorage["dataSaved"])
    {
        for (var i = 0; i < this.keys.length; i++)
        {
            var key = this.keys[i];
            this.inputs[key].value = localStorage[key];
        }
    }
}

LoveMeter.saveData = function()
{
    
    for (var i = 0; i < this.keys.length; i++)
    {
        var key = this.keys[i];
        this.settings[key] = localStorage[key] = this.inputs[key].value;
    }
    localStorage["dataSaved"] = true;
    console.log('Got options: ' + JSON.stringify(this.settings));
}

LoveMeter.submit = function()
{
    this.saveData();
    this.close();
}

LoveMeter.getQueryParam = function(variable, defaultValue)
{
    // Find all URL parameters
    var query = location.search.substring(1);
    var vars = query.split('&');
    for (var i = 0; i < vars.length; i++)
    {
        var pair = vars[i].split('=');
        // If the query variable parameter is found, decode it to use and return it for use
        if (pair[0] === variable)
        {
          return decodeURIComponent(pair[1]);
        }
    }
    return defaultValue || false;
}

LoveMeter.close = function()
{
    var return_to = this.getQueryParam('return_to', 'pebblejs://close#');
    document.location = return_to + encodeURIComponent(JSON.stringify(this.settings));
}

LoveMeter.init();

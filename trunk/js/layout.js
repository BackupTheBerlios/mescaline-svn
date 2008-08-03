function layout(widgets){

	for (var i = 0; i < widgets.length; i++) {

		var currentWidget = document.getElementById(widgets[i]);
		currentWidget.style.position = "absolute";
		currentWidget.style.top = 0 + "px";

		if (i == 0) {
			
			currentWidget.style.left = 0 + "px";
		}
		else {
			
			var previousLeft = document.getElementById(widgets[i - 1]).offsetLeft;
			var previousWidth = document.getElementById(widgets[i - 1]).offsetWidth;
			var offsetLeft = previousLeft + previousWidth;
			currentWidget.style.left = offsetLeft + "px";
		}
	}
}

function place(id, x, y){

	var currentWidget = document.getElementById(id);
	currentWidget.style.position = "absolute";
	currentWidget.style.top = y + "px";
	currentWidget.style.left = x + "px";
}

function makeMoveable(widgets, argumentString){

 	for (var i = 0; i < widgets.length; i++) {
 
		var handle = widgets[i] + "_handle";
 		var widget = new dojo.dnd.Moveable(widgets[i]);
		widget.skip = true;
		widget.handle = handle;
 		dojo.connect(widget, "onMoveStop", function(mover){
 
 			var arguments = updateArguments(widgets, argumentString);
 			window.location.replace(arguments);
		});
	}
}

function updateArguments(widgets, argumentString){

	var arguments = argumentString;

	for (var i = 0; i < widgets.length; i++) {

		var regexp = new RegExp("pos:" + widgets[i] + "=(\\\d+)x(\\\d+)");

		var widget = document.getElementById(widgets[i]);

		if (arguments.match(regexp)) { 
			arguments = arguments.replace(regexp,"pos:" + widgets[i] + "="
				+ widget.style.left.replace("px", "")
				+ "x"
				+ widget.style.top.replace("px", ""));
		}
		else {
			arguments = arguments 
				+ "&pos:" + widgets[i] + "=" 
				+ widget.style.left.replace("px", "")
				+ "x"
				+ widget.style.top.replace("px", "");
		}
	}

	return arguments;
}
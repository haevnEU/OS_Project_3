$(document).ready(function () {

	var projectName = $("div#projectname").text();
	var projectLogo = "";

	$("body").css("background-color", "#e3e3e3;").css("display", "flex").css("flex-direction", "column").css("min-height", "100vh");

	if($("td#projectlogo").length){
		$("td#projectlogo > img").css("height", "2.375rem");
		$("td#projectlogo > img").css("margin-right", "0.5rem");
		$("td#projectlogo > img").css("border-radius", "5px");
		projectLogo = $("td#projectlogo").html();
	};


	/*
	 * Adding NavBar
	 */
	$("div#main-nav").wrapAll(
		"<nav class='navbar navbar-expand-lg navbar-dark bg-dark flex-wrap'>" +
		"<div class='collapse navbar-collapse' id='navbar'>" + "</div>" +
		"</nav>");

	$("div#navbar").before(
		"<a class='navbar-brand' href='#'>" + projectLogo + projectName + "</a>" +
		"<button class='navbar-toggler' type='button' data-toggle='collapse' data-target='#navbar' aria-controls='navbar' aria-expanded='false' aria-label='Toggle navigation'>" +
		"<span class='navbar-toggler-icon'></span>" +
		"</button>");


	/*
	 * Fix dropdowns
	 */
	$("ul#main-menu").addClass("navbar-nav mr-auto").removeClass("sm sm-dox");
	$("ul#main-menu > li").addClass("nav-item");
	$("ul#main-menu > li > a").addClass("nav-link");
	$("ul#main-menu li > a.has-submenu").parent("li").addClass("dropdown");
	$("ul#main-menu li > a.has-submenu").addClass("dropdown-toggle");

	var dropDownIndex = 0;
	$("ul#main-menu li > a.has-submenu").each(function () {	
		dropDownIndex++;
		$(this).closest("li").append("<a href='#' id='dropdown" + dropDownIndex +"' class=dropdown-menu")
		$(this).closest("li").children("ul").attr("aria-labelledby", "dropdown" + dropDownIndex).addClass("dropdown-menu");
	});

	$("ul.dropdown-menu > li").addClass("dropdown-item");
	$("ul.dropdown-menu > li > a").addClass("dropdown-item");

	/*
	 * Add page briefing
	 */
	if($("div#projectbrief").length){
		$("div.header").before(
			"<div class='alert alert-dark' role='alert'>" + 
			$('div#projectbrief').text() +
			"</div>"
		);
	};


	/*
	 * Wrap page content in card
	 */
	$("div.contents").wrapAll("<div class='container-fluid' style='flex-grow: 1'><div class='row'><div class='col-12'><div class='card'><div class='card-body'></div></div></div></div></div>");

	$("div.card-body > .contents > .textblock > h1").addClass("card-title");

	$("div.contents > h3").addClass("card-title mt-4");
	$("div.contents > ul").addClass("list-group list-group-flush");
	$("div.contents > ul > li").addClass("list-group-item").css("font-family", "Courier, 'Lucida Console'");

	$("div.contents > ul > li > a").addClass("badge badge-secondary badge-pill");

	$("div.ingroups > a").addClass("badge badge-secondary badge-pill");

	/*
	 * Sidenav
	 */
	$("div#side-nav").css("min-height", "100%").css("margin-bottom", "5rem").prependTo($("#doc-content"));
	$("div#side-nav ul").addClass("list-group");
	$(".ui-resizable-e").css("background-image", "none").css("border-right", "6px solid #343a40")
	
	/*
	 * Tables
	 */
	// add bootstrap class
	$("div.contents table").addClass("table table-hover");
	// remove default class
	$("div.contents table tr").removeClass("even");

	$("div.contents table td").css("vertical-align", "middle");
	
	// fix tree alignment 

	$("div.contents table span").filter(function() {
		return $(this).attr("style") && $(this).attr("style").indexOf("width:112px;") >= 0;
	}).remove();

	$("div.contents table span").filter(function() {
		return $(this).attr("style") && $(this).attr("style").indexOf("width:96px;") >= 0;
	}).remove();

	$("div.contents table span").filter(function() {
		return $(this).attr("style") && $(this).attr("style").indexOf("width:80px;") >= 0;
	}).remove();

	$("div.contents table span").filter(function() {
		return $(this).attr("style") && $(this).attr("style").indexOf("width:64px;") >= 0;
	}).remove();

	// fix tree alignment 
	$("div.contents table span").filter(function() {
		return $(this).attr("style") && $(this).attr("style").indexOf("width:48px;") >= 0;
	}).remove();

	// fix tree alignment 
	$("div.contents table span").filter(function() {
		return $(this).attr("style") && $(this).attr("style").indexOf("width:32px;") >= 0;
	}).remove();

	// fix tree alignment 
	$("div.contents table span").filter(function() {
		return $(this).attr("style") && $(this).attr("style").indexOf("width:16px;") >= 0;
	}).remove(); 

	// fix tree alignment 
	$("div.contents table tr").filter(function() {
		return $(this).attr("style") && $(this).attr("style").indexOf("width:0px;") >= 0;
	}).remove(); 


	$("div.contents table tr[id^='row']").filter(function() { 
		if($(this).attr("id").length == 6) {
			$(this).children("td").first().css("padding-left", "1.375rem");
		} else if($(this).attr("id").length == 8) {
			$(this).children("td").first().css("padding-left", "3.375rem");
		} else if($(this).attr("id").length == 10) {
			$(this).children("td").first().css("padding-left", "5.375rem");
		}else if($(this).attr("id").length == 12) {
			$(this).children("td").first().css("padding-left", "7.375rem");
		}else if($(this).attr("id").length == 14) {
			$(this).children("td").first().css("padding-left", "9.375rem");
		}else if($(this).attr("id").length == 16) {
			$(this).children("td").first().css("padding-left", "11.375rem");
		}else if($(this).attr("id").length == 18) {
			$(this).children("td").first().css("padding-left", "13.375rem");
		}else if($(this).attr("id").length == 20) {
			$(this).children("td").first().css("padding-left", "15.375rem");
		}
	});

	// adds monospace font + fix layout
	$("div.contents table tr > td.entry").css("font-family", "Courier, 'Lucida Console'");
	$("div.contents table tr > td.entry").removeClass("entry");
	$("div.contents table tr > td.desc").attr("class", "text-muted");
	$("div.directory").removeClass("directory");
	
	// fix prefix icon (dropdown-arrow)
	$("div.contents table tr > td .arrow").attr("style", "display: inline-block; width: 1.375rem; height: 1.375rem; text-align: center; margin-left: -1.375rem;");

	$("div.contents table tr > td .icona").attr("style", "width: 1.375rem; height: 1.375rem; margin: 0 1ch 0 0;").addClass("badge badge-secondary badge-pill");
	$("div.contents table tr > td .icona > .icon").attr("style", "width: 1.375rem; height: 1.375rem; font-size: 1rem;");
	
	// adjust index table layout
	$("div.contents > table table div.ah").closest("table.table.table-hover").find("td.toggler").remove();
	$("div.contents > table table div.ah").closest("table.table.table-hover").addClass("mx-1");
	$("div.contents > table table div.ah").each(function() {
		$(this).appendTo($(this).closest("table").parent().children("a"));
		$(this).parent().parent().children("table").remove();
		$(this).parent().appendTo($(this).closest("tr"));
		$(this).parent().parent().children("td").remove();
		$(this).parent().parent().children("a").wrap("<th class='text-light bg-secondary'></th>");
		$(this).parent().parent().parent().next().remove();
	});

	$(".memdoc > p.definition").addClass("text-muted");
	$(".memname").css("margin", "0");
	$(".memitem").css("margin", "0 0 2rem 0").css("padding", "0 0 2rem 0").css("border-bottom", "1px solid #777777")
	$(".memproto").css("min-height", "4rem").css("font-size", " 1.15rem").css("font-family", "Courier, 'Lucida Console'");
	$(".memname.table tr > td").css("padding", "0");
	$("span.mlabel").css("margin-right", "1ch").addClass("badge badge-secondary badge-pill");
	$("dl.todo").addClass("alert alert-dark");
	$(".groupheader").addClass("h1 mb-5");
	$("td.fieldname").css("font-family", "Courier, 'Lucida Console'");

	$("table table td").css("border", "none");
	/*
	 * Headertitle
	 */
	$("div.headertitle").wrap("<div class='container-fluid'>" + "<div class='row'>" + "<div class='col-12'>" + "</div>" + "</div>" + "</div>") .addClass("page-header");
	$("div.title").addClass("h1");

	$(".summary").addClass("btn-group m-3").children("a").addClass("btn btn-secondary");
	$(".summary").contents().filter(function(){return this.nodeType === 3;}).remove()
	/*
	 * Navigation prefix symbols
	 */
	// Main Page
	$("li > a[href='index.html']").prepend("<i class='fas fa-home'></i> ");

	// Related Pages
	$("li > a[href='pages.html']").prepend("<i class='fas fa-folder-open'></i> ");

	// Namespaces
	$("li > a[href='namespaces.html']").prepend("<i class='fas fa-sitemap'></i> ");
	// Namespaces -> Members
	$("li > a[href='namespacemembers.html']").prepend("<i class='fa fa-bars'></i> ");
	// Namespaces -> Members -> Functions
	$("li > a[href='namespacesmembers_func.html']").prepend("<i class='fas fa-terminal'></i> ");
	// Namespaces -> Members -> Variables
	$("li > a[href='namespacesmembers_var.html']").prepend("<i class='fas fa-exchange-alt'></i> ");
	
	// Classes 
	$("li > a[href='annotated.html']").prepend("<i class='fa fa-list-ul'></i> ");
	// Classes -> Index
	$("li > a[href='classes.html']").prepend("<i class='fa fa-book'></i> ");
	// Classes -> Members
	$("li > a[href='functions.html']").prepend("<i class='fa fa-list'></i> ");
	// Classes -> Members -> Functions
	$("li > a[href='functions_func.html']").prepend("<i class='fas fa-terminal'></i> ");
	// Classes -> Members -> Variables
	$("li > a[href='functions_vars.html']").prepend("<i class='fas fa-exchange-alt'></i> ");
	// Classes -> Members -> Enumerations
	$("li > a[href='functions_enum.html']").prepend("<i class='fa fa-list'></i> ");
	// Classes -> Members -> Enumerator
	$("li > a[href='functions_eval.html']").prepend("<i class='fa fa-list'></i> ");

	// Files
	$("li > a[href='files.html']").prepend("<i class='fas fa-file'></i> ");
	// Files -> Members
	$("li > a[href='globals.html']").prepend("<i class='fa fa-list'></i> ");
	// Files -> Members -> Functions
	$("li > a[href='globals_func.html']").prepend("<i class='fas fa-terminal'></i> ");
	// Files -> Members -> Variables
	$("li > a[href='globals_vars.html']").prepend("<i class='fas fa-exchange-alt'></i> ");
	// Files -> Members -> Typedefs
	$("li > a[href='globals_type.html']").prepend("<i class='fa fa-list'></i> ");
	// Files -> Members -> Enumerations
	$("li > a[href='globals_enum.html']").prepend("<i class='fa fa-list'></i> ");
	// Files -> Members -> Enumerator
	$("li > a[href='globals_eval.html']").prepend("<i class='fa fa-list'></i> ");
	// Files -> Members -> Macros
	$("li > a[href='globals_defs.html']").prepend("<i class='fa fa-list'></i> ");


	/*
	 * Fix Searchbar design
	 */
	$("div#MSearchBox > span.right").remove();
	$("div#MSearchBox").closest("li").removeAttr("style");
	$("div#MSearchBox > .left > img#MSearchSelect").replaceWith(
		"<div class='input-group-prepend'>" + 
		"<button id='MSearchSelect' class='input-group-text btn btn-secondary' onmouseover='return searchBox.OnSearchSelectShow()' onmouseout='return searchBox.OnSearchSelectHide()'>" +
		"<i class='fas fa-search'></i>" + 
		"</button>" +
		"</test");

	// Replaces css values
	$("div#MSearchBox").css("white-space", "normal").css("float", "none").css("margin-top", "0").css("right", "auto").css("width", "auto").css("height", "auto").css("z-index", "auto");
	$("div#MSearchBox .input-group-prepend button#MSearchSelect").css("display", "block").css("position", "relative").css("width", "auto").css("height", "auto");

	$("div#MSearchBox > .left").addClass("input-group");
	$("div#MSearchBox > .left.input-group").removeClass("left");

	$("div#MSearchBox input#MSearchField").addClass("form-control").css("position", "relative").css("height", "auto").css("background", "none").css("width", "1%").css("border", "1px solid #ced4da").css("margin-left", "0").css("padding-left", "0.75rem").css("color", "#ffffff").css("border-radius", "0 0.25rem 0.25rem 0");

	$("div#MSearchBox input#MSearchField").on("change paste keyup", function(){
		if($("div#MSearchBox input#MSearchField").val().length == 0)
		{
			$("div#MSearchBox input#MSearchField").css("border-radius", "0 0.25rem 0.25rem 0");
		
		} else { 
			$("div#MSearchBox input#MSearchField").css("border-radius", "0");
			$("div#MSearchBox a#MSearchClose").css("display", "block");
		}
	});

	// Fix Close Search Button
	$("div#MSearchBox input#MSearchField").after("<div class='input-group-append'><a type='button' id='MSearchClose' class='input-group-text btn btn-secondary' href='javascript:searchBox.CloseResultsWindow()'><i class='fas fa-times-circle'></i></a></div>");
	// overwrite default styles, add hover effect
	$("div#MSearchBox a#MSearchClose").css("position", "relative").css("top", "auto").css("border", "1px solid #ced4da").css("margin", "0").css("padding", "0.375rem 0.75rem").css("background", "#e9ecef").mouseenter(function(){
		$("div#MSearchBox a#MSearchClose").css("background", "#5a6268").css("border-color", "#545b62");
	}).mouseleave(function(){
		$("div#MSearchBox a#MSearchClose").css("background", "#e9ecef").css("border-color", "#ced4da");
	}).click(function(){
		$("div#MSearchBox input#MSearchField").css("border-radius", "0 0.25rem 0.25rem 0");
	});

	// fix search result window positioning
	$("div#MSearchResultsWindow").wrapAll("<div class='MSearchResultsWindowWrapper' style='position: absolute; top: 9px; left: 372px;'></div>");


	/*
	 * Lists
	 */
	// add hover effect to list elements
	$("li.list-group-item").mouseenter(function(){
		$(this).addClass("active")
	}).mouseleave(function(){
		$("li.list-group-item").removeClass("active")
	});

	/*
	 * Iframe
	 */
	$("iframe").attr("scrolling", "yes");


	/*
	 * Details level selection
	 */
	$("div.levels").wrapAll("<div class='btn-toolbar' role='toolbar'></div>");
	$("div.levels").addClass("btn-group my-1");
	$("div.levels").contents().filter(function(){
		return this.nodeType === 3;
	}).remove();
	$("div.levels").prepend("<button class='btn btn-default btn-secondary btn-xs' disabled>Detail Level</button>");
	$("div.levels > span").addClass("btn btn-default btn-secondary btn-xs");

	/*
	 * Index
	 */
	$("div.qindex").wrap("<div class='btn-toolbar' role='toolbar'></div>");
	$("div.qindex").addClass("btn-group m-1");
	$("div.qindex").contents().filter(function(){
		return this.nodeType === 3;
	}).remove();
	$("div.qindex > a.qindex").addClass("btn btn-default btn-secondary btn-xs");

	/*
	 * Page Footer
	 */
	$("hr.footer").remove();
	$("address.footer").css("margin-bottom", "0").wrap(
		"<footer class='footer mt-2' style='position: static; bottom: 0; width: 100%; height: 60px; line-height: 60px; background-color: #f5f5f5;'>" + 
	"<div class='container'></div></div>");

	$("footer div.container").add($(this).find("small"));
	

	/* enumerations */
	$("table.fieldtable").removeClass("fieldtable").addClass("table table-striped table-bordered").each(function () {
		$(this).prepend("<thead></thead>");
		$(this).find("tbody > tr:first").prependTo($(this).find("thead"));

		$(this).find("td > span.success").parent().addClass("success");
		$(this).find("td > span.warning").parent().addClass("warning");
		$(this).find("td > span.danger").parent().addClass("danger");
	});

	/*
	 * titlearea is redundant now.
	 */
	$("div#titlearea").remove();

	if(window.location.hash) {
		$(window).scrollTop($(window.location.hash).offset().top).scrollLeft($(window.location.hash).offset().left);
	}
});

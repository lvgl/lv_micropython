import * as monaco from "monaco-editor/esm/vs/editor/editor.main.js";
import { GoldenLayout, LayoutConfig } from "golden-layout";
import { Terminal } from "xterm";
import { FitAddon } from "xterm-addon-fit";
import { WebLinksAddon } from 'xterm-addon-web-links';
import "./golden-layout.less";
import "xterm/css/xterm.css";
import "./styles.css";
import debounce from "debounce";

var total, final_script;

/**
 * Add or update a query string parameter. If no URI is given, we use the current
 * window.location.href value for the URI.
 *
 * Based on the DOM URL parser described here:
 * http://james.padolsey.com/javascript/parsing-urls-with-the-dom/
 *
 * @param   (string)    uri     Optional: The URI to add or update a parameter in
 * @param   (string)    key     The key to add or update
 * @param   (string)    value   The new value to set for key
 *
 * Tested on Chrome 34, Firefox 29, IE 7 and 11
 */
function update_query_string(uri, key, value) {
    // Use window URL if no query string is provided
    if (!uri) {
        uri = window.location.href;
    }

    // Create a dummy element to parse the URI with
    var a = document.createElement("a"),
        // match the key, optional square brackets, an equals sign or end of string, the optional value
        reg_ex = new RegExp(key + "((?:\\[[^\\]]*\\])?)(=|$)(.*)"),
        // Setup some additional variables
        qs,
        qs_len,
        key_found = false;

    // Use the JS API to parse the URI
    a.href = uri;

    // If the URI doesn't have a query string, add it and return
    if (!a.search) {
        a.search = "?" + key + "=" + value;

        return a.href;
    }

    // Split the query string by ampersands
    qs = a.search.replace(/^\?/, "").split(/&(?:amp;)?/);
    qs_len = qs.length;

    // Loop through each query string part
    while (qs_len > 0) {
        qs_len--;

        // Remove empty elements to prevent double ampersands
        if (!qs[qs_len]) {
            qs.splice(qs_len, 1);
            continue;
        }

        // Check if the current part matches our key
        if (reg_ex.test(qs[qs_len])) {
            // Replace the current value
            qs[qs_len] = qs[qs_len].replace(reg_ex, key + "$1") + "=" + value;

            key_found = true;
        }
    }

    // If we haven't replaced any occurrences above, add the new parameter and value
    if (!key_found) {
        qs.push(key + "=" + value);
    }

    // Set the new query string
    a.search = "?" + qs.join("&");

    return a.href;
}

var editor_cmd = 0;

var snippetbin_url = "https://forum.lvgl.io:8443";

const query_revision = "script_direct";

window.set_dirty = function () {
    document.querySelectorAll(".dirty_watcher").forEach((e) => e.classList.add("dirty"));
    let link = document.getElementById("link");
    link.textContent = "Link to online script (you have unsaved changes)";
};

window.clear_dirty = function () {
    document.querySelectorAll(".dirty_watcher.dirty").forEach((e) => e.classList.remove("dirty"));
    link.textContent = "Link to online script";
};

var cache = {};
function cached_get(url) {
    if (url in cache) return Promise.resolve(cache[url]);
    else {
        return axios.get(url).then((res) => {
            cache[url] = res;
            return res;
        });
    }
}

function load_revision(revision, update_history, cb) {
    if (!revision) return;
    const $modal = $("#snippetbin-modal");
    let timeout = setTimeout(() => {
        timeout = null;
        console.log("SHOW");
        $modal.modal('show');
    }, 500);
    cached_get(snippetbin_url + "/load_file/" + revision)
        .then((res) => {
            if(timeout != null) {
                clearTimeout(timeout);
            }
            if($modal.data("modal-shown"))
                $modal.modal('hide');
            else
                $modal.one("shown.bs.modal", () => $modal.modal('hide'));
            current_revision = revision;
            editor_cmd++;
            setEditorValue(res.data.text);
            if (update_history) {
                let history = document.getElementById("history");
                let revision_history_data = document.getElementById("history_data");
                while (revision_history_data.firstChild) {
                    revision_history_data.removeChild(revision_history_data.firstChild);
                }
                res.data.history.forEach((item, i) => {
                    let option = document.createElement("option");
                    option.value = i;
                    option.dataset.value = item;
                    revision_history_data.appendChild(option);
                });
                history.max = res.data.history.length - 1;
                history.value = 0;
            }
            let link = document.getElementById("link");
            link.setAttribute("href", update_query_string(window.location.href, query_revision, current_revision));
            link.textContent = "Link to online script";
            clear_dirty();
            if (cb != undefined) cb();
        })
        .catch((err) => {
            console.log(err);
        });
}

window.load_revision = load_revision;

window.save = function (event) {
    event.target.disabled = true;
    const span = event.target.childNodes[2];
    span.textContent = "Saving...";
    const fin = () => {
        span.textContent = "Save";
        event.target.disabled = false;
    };
    axios
        .post(snippetbin_url + "/save_file", {
            text: getEditorValue(),
            original_revision: current_revision,
        })
        .then((res) => {
            window.load_revision(res.data.revision, true);
            fin();
        })
        .catch((err) => {
            console.log("ERROR: " + err.message);
            fin();
        });
};

var external_link = null;
/** @type {monaco.editor.IStandaloneCodeEditor} */
var editor;
/** @type {HTMLIFrameElement} */
var iframe;
/** @type {Terminal} */
var term;
var script_passed = false;
var current_revision = null;

function forward_event(event) {
    if (iframe !== undefined && iframe !== null) {
        if (iframe.contentWindow !== null) iframe.contentWindow.document.dispatchEvent(new MouseEvent("mouseup"));
    }
}

document.onmouseup = forward_event;

/* MONACO COMPAT */
function getEditorValue() {
    return editor.getValue();
}
function setEditorValue(val) {
    editor.setValue(val);
}

/* END MONACO COMPAT */
window.runScript = function () {
    var $this = $("#run-button");
    $this.prop("disabled", "disabled");
    term.write("\x1bc");

    const context = document.getElementById("canvas").getContext("2d");

    context.clearRect(0, 0, canvas.width, canvas.height);

    iframe.setAttribute("data-cscript", LZString.compressToEncodedURIComponent(getEditorValue()));

    clear_iframe(iframe);
    iframe.src = "lvgl.html"; /*+ "&timestamp=" + new Date().getTime()*/
    iframe.contentWindow.location.href = iframe.src;
    console.log("Iframe src: " + iframe.src, "top", window.parent);
    term.setOption("disableStdin", true);
    term.write("Loading...\r");
};
window.reenableButton = function () {
    $("#run-button").removeProp("disabled");
    if (term != undefined && term != null) term.setOption("disableStdin", false);
};

function processScriptArg(url, lzstring) {
    var script_passed_handler = function () {
        console.log("Script passed: " + script_passed);
        runScript();
        script_passed = false;
    };
    if (!lzstring) {
        // if url is a single url we convert it to array
        if (!Array.isArray(url)) {
            url = [{ url: url }];
        }
        var prev_external_link = external_link;
        var request = null;

        var error_handler = function () {
            if (prev_external_link != null) {
                external_link = prev_external_link;
            }
            if (request != null)
                alert(
                    "Failed to load script due to error " +
                        request.status +
                        ": " +
                        request.statusText +
                        "\nThe contents of the external link box have been reverted."
                );
            else alert("The URL you passed is invalid.");
        };

        try {
            url.forEach((e) => {
                new URL(e.url);
            });
        } catch (e) {
            error_handler();
            return;
        }

        // last url is probably the important one
        external_link = url[url.length - 1].url;
        // read text from URL location
        url.forEach((e) => {
            let request = new XMLHttpRequest();
            console.log("GET " + e.url);
            request.overrideMimeType("text/plain");
            request.open("GET", e.url, true);
            request.send(null);
            request.onerror = error_handler;
            request.onreadystatechange = function () {
                if (request.readyState === 4) {
                    if (request.status == 200) {
                        console.log(request.reponseText);
                        if (request.responseText === undefined) return;
                        e.content = request.responseText;
                        // check if all of them have content
                        total = url.reduce((total, el) => total + ("content" in el), 0);
                        console.log(total + " of " + url.length + " loaded");
                        if (total == url.length) {
                            final_script = "";
                            url.forEach((e) => {
                                if ("comment" in e) {
                                    final_script += "##### " + e.comment + " #####\n\n";
                                }
                                final_script += e.content + "\n\n";
                            });
                            setEditorValue(final_script);
                            script_passed_handler();
                        }
                    } else {
                        error_handler();
                    }
                }
            };
        });
    } else {
        external_link = null;
        // decompress LZString
        current_revision = url;
        window.load_revision(current_revision, true, function () {
            script_passed_handler();
        });
    }
}
function getSearchArg(argname) {
    /* Run custom script if passed */
    var custom = undefined;
    try {
        custom = new URL(window.location.href).searchParams.get(argname);
    } catch (e) {
        console.log(e + ": URL seems to be unsupported");
    }
    return custom;
}
function clear_iframe(iframe) {
    iframe.contentWindow.document.open();
    iframe.contentWindow.document.write("");
    iframe.contentWindow.document.close();
}

$(window).load(function () {
    $("#snippetbin-modal").on("shown.bs.modal", function(e) {
        $(this).data("modal-shown", true);
    });
    $("#snippetbin-modal").on("hidden.bs.modal", function(e) {
        $(this).data("modal-shown", false);
    })
    $(document).on("shown.bs.tooltip", function (e) {
        setTimeout(function () {
            $(e.target).tooltip("hide");
        }, 5000);
    });
    window.clear_dirty();
    /* Enable tooltips */
    $('[data-toggle="tooltip"]').tooltip();

    /** @type {LayoutConfig} */
    const layoutConfig = {
        header: {
            popout: false,
        },
        content: [
            {
                type: "row",
                content: [
                    {
                        type: "component",
                        componentName: "Editor",
                        componentState: { label: "Editor" },
                    },
                    {
                        type: "column",
                        content: [
                            {
                                type: "component",
                                componentName: "Display",
                                componentState: { label: "Display" },
                            },
                            {
                                type: "component",
                                componentName: "REPL",
                                componentState: { label: "REPL" },
                            },
                        ],
                    },
                ],
            },
        ],
    };
    /** @type {HTMLDivElement} */
    const appContainer = document.getElementById("app-container");
    const myLayout = new GoldenLayout(appContainer);

    myLayout.registerComponentFactoryFunction("Editor", function (container, componentState) {
        /** @type {monaco.editor.IStandaloneCodeEditor} */
        let ed = null;
        const onResize = debounce(() => {
            if (ed != null) ed.layout();
        }, 50);
        ed = monaco.editor.create(container.element, {
            value: `
# Initialize

import display_driver
import lvgl as lv

# Create a button with a label

scr = lv.obj()
btn = lv.button(scr)
btn.align(lv.ALIGN.CENTER, 0, 0)
label = lv.label(btn)
label.set_text('Hello World!')
lv.screen_load(scr)
    `,
            language: "python",
            theme: "vs-dark",
            automaticLayout: false, // the important part
        });
        container.addEventListener("resize", onResize);
        editor = ed;
        editor.onDidChangeModelContent(() => {
            if (editor_cmd) editor_cmd--;
            else window.set_dirty();
        });
    });
    /** @type {HTMLIFrameElement} */
    myLayout.registerComponentFactoryFunction("Display", function (container, componentState) {
        container.element.classList.add("canvas-container");
        const canvas = document.createElement("canvas");
        canvas.setAttribute("id", "canvas");
        canvas.setAttribute("tabindex", "-1");
        canvas.addEventListener("click", () => {
            canvas.focus()
        });
        container.element.appendChild(canvas);
        const resizeHandler = debounce(() => {
            let canvasW = parseInt(canvas.getAttribute("width"));
            let canvasH = parseInt(canvas.getAttribute("height"));
            if(isNaN(canvasW) || isNaN(canvasH)) {
                canvasW = 420;
                canvasH = 320;
            }
            let ratio = container.width / canvasW;
            if (canvasH * ratio > container.height) {
                ratio = container.height / canvasH;
            }
            ratio = Math.min(ratio, 1);
            canvas.style.width = Math.round(canvasW * ratio) + "px";
            canvas.style.height = Math.round(canvasH * ratio) + "px";
        }, 50);
        container.on("resize", resizeHandler);
        const observer = new MutationObserver(resizeHandler);

        // call `observe()` on that MutationObserver instance,
        // passing it the element to observe, and the options object
        observer.observe(canvas, {attributes: true, attributeFilter: ["width", "height"]});
    });

    myLayout.registerComponentFactoryFunction("REPL", function (container, componentState) {
        term = new Terminal({
            tabStopWidth: 8,
            cursorBlink: true,
            cursorStyle: "block",
            applicationCursor: true,
            theme: {
                background: "#1e1e1e",
            },
        });
        const fitAddon = new FitAddon();
        term.loadAddon(fitAddon);
        term.loadAddon(new WebLinksAddon());
        term.open(container.element);
        container.on(
            "resize",
            debounce(() => {
                try {
                    fitAddon.fit();
                } catch (e) {
                    console.error(e);
                }
            }, 50)
        );
        function xterm_helper(key) {
            function ESC(data) {
                return String.fromCharCode(27) + data;
            }
            if (key.charCodeAt(0) == 12) {
                var cy = 0 + term.buffer.cursorY;
                if (cy > 0) {
                    if (cy <= term.rows) {
                        term.write(ESC("[B"));
                        term.write(ESC("[J"));
                        term.write(ESC("[A"));
                    }

                    term.write(ESC("[A"));
                    term.write(ESC("[K"));
                    term.write(ESC("[1J"));

                    for (var i = 1; i < cy; i++) {
                        term.write(ESC("[A"));
                        term.write(ESC("[M"));
                    }
                    term.write(ESC("[M"));
                }
                return false;
            }
            return true;
        }
        term.onData(function (key) {
            if (xterm_helper(key)) {
                for (var i = 0; i < key.length; i++) {
                    if (iframe.contentWindow !== null) {
                        let c = key.charCodeAt(i);
                        iframe.contentWindow.process_char(c);
                    }
                }
            }
        });
        window.addEventListener(
            "micropython-print",
            (e) => {
                term.write(e.detail);
            },
            false
        );
    });

    iframe = document.getElementById("emscripten-iframe");
    iframe.src = "about:blank";
    iframe.setAttribute("data-lv-bindings-commit-hash", LV_BINDINGS_COMMIT_HASH);
    iframe.contentWindow.location.href = iframe.src;
    clear_iframe(iframe);
    myLayout.loadLayout(layoutConfig);

    window.addEventListener(
        "resize",
        debounce(() => {
            myLayout.setSize(appContainer.offsetWidth, appContainer.offsetHeight);
        })
    );

    var script = getSearchArg("script");
    var script_direct = getSearchArg("script_direct");
    var script_startup = getSearchArg("script_startup");
    if (script_direct !== undefined && script_direct !== null) {
        script_passed = true;
        processScriptArg(script_direct, true);
    } else if (script_startup !== undefined && script_startup !== null) {
        // both startup script and normal script are passed
        if (script !== undefined && script !== null) {
            script_passed = true;
            // pass an array of objects
            processScriptArg([
                {
                    comment: "startup script",
                    url: script_startup,
                },
                {
                    comment: "main script",
                    url: script,
                },
            ]);
        } else {
            script_passed = true;
            processScriptArg(script_startup);
        }
    } else if (script !== undefined && script !== null) {
        script_passed = true;
        processScriptArg(script);
    } else runScript();
});

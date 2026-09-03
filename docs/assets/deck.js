/* =========================================================================
   deck.js  --  埋め込みMarkdownを読み込んでスライド化する小さなエンジン
   依存ライブラリなし。file:// で開いてもそのまま動く（ネット不要）。

   各章HTMLの中に
     <script type="text/markdown" id="deck-source"> ...Markdown... </script>
   を置いておくと、このスクリプトがそれをスライドに変換します。

   Markdown 記法（このワークショップ用の最小セット）:
     ---            スライドの区切り（行頭・単独）
     # 〜 ######    見出し（各スライド最初の # がスライドタイトル）
     1. / - / *     番号つき／箇条書きリスト（手順）
     ![説明](画像)   画像（説明はキャプションになる。連続すると横並び）
     > 本文          博士の吹き出し（先頭に ⚠ を付けると注意色）
                    博士のアイコンは本文の長さで自動に切り替わる
                    （短い→顔だけ ／ 長い→全身）
     >> 本文         長さに関係なく全身の博士にする（見せ場用）
     >| 本文         長さに関係なく顔だけの博士にする
     :::            スライド内を左右カラムに分ける区切り
     ```js …  ```   コードブロック（色分け＋行番号＋コピーボタン）
     **太字** *斜体* `コード` [リンク](url)
   スライドの先頭に {cover} と書くと表紙用の中央寄せレイアウトになる。
   ========================================================================= */
(function () {
  "use strict";

  // 吹き出しに出る博士の絵（章ページは1階層下なので ../ から）
  var MASCOT_FULL = "../assets/hakase.png";      // 全身。縦に長いので長文の吹き出し向き
  var MASCOT_FACE = "../assets/hakase-face.png"; // 顔だけ。1～2行の短い吹き出し向き
  // 本文がこの文字数以上になったら全身にする（>> ／ >| で個別に上書きできる）
  var MASCOT_FULL_MIN = 60;

  // ---- インライン記法 ---------------------------------------------------
  function esc(s) {
    return s.replace(/&/g, "&amp;").replace(/</g, "&lt;").replace(/>/g, "&gt;");
  }
  function inline(s) {
    s = esc(s);
    // 画像（インライン）
    s = s.replace(/!\[([^\]]*)\]\(([^)]+)\)/g,
      '<img src="$2" alt="$1">');
    // リンク
    s = s.replace(/\[([^\]]+)\]\(([^)]+)\)/g,
      '<a href="$2" target="_blank" rel="noopener">$1</a>');
    // 太字 → コード → 斜体 の順（太字を先に処理して * の衝突を避ける）
    s = s.replace(/\*\*([^*]+)\*\*/g, "<strong>$1</strong>");
    s = s.replace(/`([^`]+)`/g, "<code>$1</code>");
    s = s.replace(/\*([^*]+)\*/g, "<em>$1</em>");
    return s;
  }

  // 1行が「画像だけ」かどうか
  function isImageLine(line) {
    return /^!\[[^\]]*\]\([^)]+\)\s*$/.test(line.trim());
  }
  function imageHTML(line) {
    var m = line.trim().match(/^!\[([^\]]*)\]\(([^)]+)\)\s*$/);
    var alt = m[1], src = m[2];
    var cap = alt ? '<figcaption>' + inline(alt) + '</figcaption>' : '';
    return '<figure><img src="' + src + '" alt="' + esc(alt) + '">' + cap + '</figure>';
  }

  // ---- コードブロック（```js … ```） -------------------------------------
  //   ```js  キャプション        ← 言語（js / python）と、右に出す小見出し
  //   basic.showIcon(IconNames.Heart)
  //   ```
  // 色分け・行番号・「コピー」ボタン付きのカードにする。
  //   Web（パソコン）… ボタンでワンクリックコピー
  //   スマホ          … 文字を読めるサイズに拡大（CSS側で px 指定）
  //   発表モード・印刷 … ボタンは消え、行数に応じて自動で文字サイズを調整
  var LANG_LABEL = {
    cpp: "Arduino (C++)", ino: "Arduino (C++)", arduino: "Arduino (C++)", c: "Arduino (C++)",
    js: "JavaScript", javascript: "JavaScript", ts: "JavaScript", typescript: "JavaScript",
    py: "Python", python: "Python", "": "コード"
  };
  var KEYWORDS = {
    cpp: /^(void|int|long|float|double|char|bool|byte|unsigned|const|static|if|else|for|while|do|switch|case|default|break|continue|return|true|false|struct|class|new|delete|sizeof|include|define)$/,
    js: /^(let|const|var|function|return|if|else|for|while|do|break|continue|new|of|in|switch|case|default|class|this|true|false|null|undefined)$/,
    py: /^(def|return|if|elif|else|for|while|break|continue|import|from|as|in|is|and|or|not|class|with|pass|lambda|global|True|False|None)$/
  };
  // Arduino / micro:bit でよく出てくる関数・グループ名。青くして目立たせる
  var NAMESPACES = /^(pinMode|digitalWrite|digitalRead|analogRead|analogWrite|analogReadResolution|analogWriteResolution|analogWriteFrequency|delay|delayMicroseconds|millis|micros|tone|noTone|pulseIn|map|constrain|random|setup|loop|Serial|Wire|SPI|EEPROM|Servo|basic|input|music|led|radio|pins|control|serial|console|Math)$/;

  function langKey(lang) {
    if (/^(cpp|c|ino|arduino|c\+\+)$/.test(lang)) return "cpp";
    return /^(py|python)$/.test(lang) ? "py" : "js";
  }

  // 1行を色分けする（複数行にまたがる /* */ は使わない前提の簡易版）
  function hiLine(line, lk) {
    var re = lk === "py"
      ? /(#.*)|("(?:[^"\\]|\\.)*"|'(?:[^'\\]|\\.)*')|(\b\d+(?:\.\d+)?\b)|([A-Za-z_][A-Za-z0-9_]*)/g
      : /(\/\/.*)|("(?:[^"\\]|\\.)*"|'(?:[^'\\]|\\.)*'|`(?:[^`\\]|\\.)*`)|(\b\d+(?:\.\d+)?\b)|([A-Za-z_$][A-Za-z0-9_$]*)/g;
    var out = "", last = 0, m;
    while ((m = re.exec(line)) !== null) {
      out += esc(line.slice(last, m.index));
      last = re.lastIndex;
      if (m[1]) out += '<span class="t-com">' + esc(m[1]) + "</span>";          // コメント
      else if (m[2]) out += '<span class="t-str">' + esc(m[2]) + "</span>";     // 文字列
      else if (m[3]) out += '<span class="t-num">' + esc(m[3]) + "</span>";     // 数字
      else {
        var w = m[4], cls = "";
        if (KEYWORDS[lk].test(w)) cls = "t-key";                                 // let / function …
        else if (NAMESPACES.test(w)) cls = "t-ns";                               // basic / input …
        else if (/^[A-Z]/.test(w)) cls = "t-type";                               // IconNames / Button …
        else if (line.charAt(re.lastIndex) === "(") cls = "t-fn";                // showIcon( …
        out += cls ? '<span class="' + cls + '">' + esc(w) + "</span>" : esc(w);
      }
    }
    return out + esc(line.slice(last));
  }

  function codeCard(code, lang, caption) {
    var lk = langKey(lang);
    var lines = code.replace(/\s+$/, "").split("\n");
    var body = lines.map(function (l) {
      return '<div class="cl">' + hiLine(l, lk) + "</div>";
    }).join("");
    // いちばん長い行の文字数（全角は2文字ぶんで数える）
    var cols = 0;
    lines.forEach(function (l) {
      var w = 0;
      for (var k = 0; k < l.length; k++) w += l.charCodeAt(k) > 0x2e80 ? 2 : 1;
      if (w > cols) cols = w;
    });
    // --code-lines / --code-cols … 行数と長さ。CSS がこれを見て文字サイズを決める
    //   （16:9のスライドから、たてにも よこにも はみ出さないように）
    return '<div class="code-card" data-lang="' + lk +
             '" style="--code-lines:' + lines.length + ';--code-cols:' + (cols || 1) + '">' +
             '<div class="code-head">' +
               '<span class="lang">' + esc(LANG_LABEL[lang] || LANG_LABEL[""]) + "</span>" +
               '<span class="cap">' + (caption ? inline(caption) : "") + "</span>" +
               '<button class="code-copy" type="button">📋 コピー</button>' +
             "</div>" +
             "<pre>" + body + "</pre>" +
           "</div>";
  }

  // ---- ブロック記法（1カラム分のMarkdown → HTML） ---------------------
  function blocks(md) {
    var lines = md.replace(/\r/g, "").split("\n");
    var out = [];
    var i = 0;
    while (i < lines.length) {
      var line = lines[i];

      if (line.trim() === "") { i++; continue; }

      // コードブロック（```js キャプション … ```）
      var fence = line.match(/^```+\s*([A-Za-z+#-]*)\s*(.*)$/);
      if (fence) {
        var buf = [];
        i++;
        while (i < lines.length && !/^```/.test(lines[i])) { buf.push(lines[i]); i++; }
        i++;  // 閉じの ```
        out.push(codeCard(buf.join("\n"), fence[1].toLowerCase(), fence[2].trim()));
        continue;
      }

      // 見出し
      var h = line.match(/^(#{1,6})\s+(.*)$/);
      if (h) {
        var lv = h[1].length;
        out.push("<h" + lv + ">" + inline(h[2]) + "</h" + lv + ">");
        i++; continue;
      }

      // 画像（連続していれば横並びの figrow に）
      if (isImageLine(line)) {
        var figs = [];
        while (i < lines.length && isImageLine(lines[i])) {
          figs.push(imageHTML(lines[i])); i++;
        }
        if (figs.length === 1) out.push(figs[0]);
        else out.push('<div class="figrow">' + figs.join("") + "</div>");
        continue;
      }

      // 吹き出し（blockquote）
      if (/^>\s?/.test(line)) {
        // アイコンの指定は先頭行だけで見る（>> ＝全身、>| ＝顔だけ）
        var pin = /^>>/.test(line) ? "full" : /^>\|/.test(line) ? "face" : "";
        var buf = [];
        while (i < lines.length && /^>\s?/.test(lines[i])) {
          // 先頭行にだけ指定記号が付いているので、その分も取り除く
          var mark = (pin && buf.length === 0) ? /^>[>|]\s?/ : /^>\s?/;
          buf.push(lines[i].replace(mark, "")); i++;
        }
        var text = buf.join(" ").trim();
        var cls = "";
        if (/^(⚠|！|注意)/.test(text)) { cls = " warn"; text = text.replace(/^(⚠️?|！|注意[:：]?)\s*/, ""); }
        else if (/^💪/.test(text)) { cls = " challenge"; text = text.replace(/^💪\s*/, ""); }
        // 指定がなければ本文（記号を除いたあと）の長さで決める
        var full = pin ? pin === "full" : text.length >= MASCOT_FULL_MIN;
        out.push(
          '<div class="callout' + cls + (full ? " icon-full" : " icon-face") + '">' +
            '<img class="hakase ' + (full ? "full" : "face") + '"' +
              ' src="' + (full ? MASCOT_FULL : MASCOT_FACE) + '" alt="博士">' +
            '<div class="callout-body"><p>' + inline(text) + "</p></div>" +
          "</div>");
        continue;
      }

      // 番号つきリスト
      if (/^\d+\.\s+/.test(line)) {
        var oli = [];
        while (i < lines.length && /^\d+\.\s+/.test(lines[i])) {
          oli.push("<li>" + inline(lines[i].replace(/^\d+\.\s+/, "")) + "</li>"); i++;
        }
        out.push("<ol>" + oli.join("") + "</ol>");
        continue;
      }

      // 箇条書き
      if (/^[-*]\s+/.test(line)) {
        var uli = [];
        while (i < lines.length && /^[-*]\s+/.test(lines[i])) {
          uli.push("<li>" + inline(lines[i].replace(/^[-*]\s+/, "")) + "</li>"); i++;
        }
        out.push("<ul>" + uli.join("") + "</ul>");
        continue;
      }

      // 段落（空行まで）
      var para = [];
      while (i < lines.length && lines[i].trim() !== "" &&
             !/^(#{1,6}\s|>\s?|\d+\.\s|[-*]\s|```)/.test(lines[i]) && !isImageLine(lines[i])) {
        para.push(lines[i]); i++;
      }
      out.push("<p>" + inline(para.join(" ")) + "</p>");
    }
    return out.join("\n");
  }

  // ---- スライド1枚を組み立てる -----------------------------------------
  function buildSlide(md, index) {
    var cover = false;
    md = md.replace(/^\s*\{cover\}\s*$/m, function () { cover = true; return ""; });

    // {qr: パス} … スライド右下にQRコードと説明を表示（主に表紙で使う）
    var qr = null;
    md = md.replace(/\{qr:\s*([^}]+)\}/, function (_, p) { qr = p.trim(); return ""; });

    // 画像だけで構成されたチャンクか（＝大きく見せるメディアカラム）
    function isMediaChunk(c) {
      var ls = c.replace(/\r/g, "").split("\n").filter(function (l) { return l.trim() !== ""; });
      return ls.length > 0 && ls.every(isImageLine);
    }
    // コードブロックだけのチャンクか（＝少し広く取るコードカラム）
    function isCodeChunk(c) {
      var t = c.replace(/\r/g, "").trim();
      return /^```/.test(t) && /```$/.test(t);
    }

    var inner;
    if (/^\s*:::\s*$/m.test(md)) {
      // 最初の ::: より前は「全幅ヘッダー」（タイトルなど）。残りを左右カラムにする。
      var chunks = md.split(/^\s*:::\s*$/m);
      var header = chunks.shift();
      var headerHTML = header.trim() ? blocks(header) : "";
      var cols = chunks.map(function (c) {
        var extra = isMediaChunk(c) ? " col-media" : (isCodeChunk(c) ? " col-code" : "");
        return '<div class="col' + extra + '">' + blocks(c) + "</div>";
      }).join("");
      inner = headerHTML + '<div class="cols">' + cols + "</div>";
    } else {
      inner = blocks(md);
    }

    var slide = document.createElement("section");
    slide.className = "slide" + (qr ? " has-qr" : "");
    slide.innerHTML =
      '<div class="slide-inner' + (cover ? " cover" : "") + '">' + inner + "</div>" +
      (qr ? '<div class="slide-qr"><img src="' + qr + '" alt="QRコード">' +
            '<span class="cap">この資料はこちらから<br>ダウンロードできます</span></div>' : "") +
      '<div class="page-no">' + (index + 1) + "</div>";
    return slide;
  }

  // ---- コードの「コピー」ボタン（Web閲覧時） -----------------------------
  // クリップボードAPIが使えないとき（http:// や file:// で開いたときなど）は
  // 昔ながらの execCommand("copy") に切りかえる。
  function writeClipboard(text) {
    if (navigator.clipboard && navigator.clipboard.writeText && window.isSecureContext) {
      return navigator.clipboard.writeText(text);
    }
    return new Promise(function (resolve, reject) {
      var ta = document.createElement("textarea");
      ta.value = text;
      ta.setAttribute("readonly", "");
      ta.style.cssText = "position:fixed;top:-1000px;left:0;opacity:0;";
      document.body.appendChild(ta);
      ta.select();
      try { ta.setSelectionRange(0, text.length); } catch (e) {}
      var ok = false;
      try { ok = document.execCommand("copy"); } catch (e) {}
      document.body.removeChild(ta);
      ok ? resolve() : reject();
    });
  }

  function setupCopyButtons(root) {
    var btns = root.querySelectorAll(".code-copy");
    Array.prototype.forEach.call(btns, function (btn) {
      btn.addEventListener("click", function (e) {
        e.stopPropagation();          // 発表モードの「クリックで次へ」を邪魔しない
        e.preventDefault();
        var card = btn.parentNode.parentNode;   // .code-copy → .code-head → .code-card
        var text = Array.prototype.map.call(
          card.querySelectorAll("pre .cl"),
          function (l) { return l.textContent; }
        ).join("\n");

        clearTimeout(btn.__t);
        writeClipboard(text).then(function () {
          btn.textContent = "✅ コピーしました";
          btn.className = "code-copy done";
        }, function () {
          btn.textContent = "⚠ 手でコピーしてね";
          btn.className = "code-copy fail";
          var r = document.createRange();
          r.selectNodeContents(card.querySelector("pre"));
          var sel = window.getSelection();
          sel.removeAllRanges(); sel.addRange(r);
        });
        btn.__t = setTimeout(function () {
          btn.textContent = "📋 コピー";
          btn.className = "code-copy";
        }, 1800);
      });
    });
  }

  // ---- 初期化 -----------------------------------------------------------
  function init() {
    var src = document.getElementById("deck-source");
    if (!src) return;
    var md = src.textContent;

    // スライド分割（行頭の --- 単独）
    var parts = md.replace(/\r/g, "").split(/\n[ \t]*-{3,}[ \t]*\n/);

    var deck = document.getElementById("deck") || (function () {
      var d = document.createElement("div"); d.id = "deck"; d.className = "deck";
      document.body.appendChild(d); return d;
    })();

    var slides = [];
    parts.forEach(function (p, idx) {
      if (p.trim() === "") return;
      var el = buildSlide(p, slides.length);
      deck.appendChild(el);
      slides.push(el);
    });

    setupCopyButtons(deck);
    setupModes(slides);

    // #present で開いたら発表モードで起動（会場配布URLに便利）。#present=3 で3枚目から。
    var mp = location.hash.match(/^#present(?:=(\d+))?$/);
    if (mp) {
      var btn = document.getElementById("btn-present");
      if (btn) btn.click();
      if (mp[1] && window.__deckGoto) window.__deckGoto(parseInt(mp[1], 10) - 1);
    }
  }

  // ---- Web / 発表モードの切り替えと操作 ---------------------------------
  function setupModes(slides) {
    var current = 0;

    function show(n) {
      current = Math.max(0, Math.min(slides.length - 1, n));
      slides.forEach(function (s, i) { s.classList.toggle("active", i === current); });
      var c = document.getElementById("counter");
      if (c) c.textContent = (current + 1) + " / " + slides.length;
      var active = slides[current];
      if (active && !document.body.classList.contains("mode-present")) {
        active.scrollIntoView({ behavior: "smooth", block: "center" });
      }
    }

    function enterPresent() {
      document.body.classList.add("mode-present");
      // いま画面に一番近いスライドから開始
      var best = 0, bestDist = Infinity;
      slides.forEach(function (s, i) {
        var d = Math.abs(s.getBoundingClientRect().top);
        if (d < bestDist) { bestDist = d; best = i; }
      });
      show(best);
      if (document.documentElement.requestFullscreen) {
        document.documentElement.requestFullscreen().catch(function () {});
      }
    }
    function exitPresent() {
      document.body.classList.remove("mode-present");
      if (document.fullscreenElement && document.exitFullscreen) {
        document.exitFullscreen().catch(function () {});
      }
      slides[current].scrollIntoView({ block: "center" });
    }

    window.__deckGoto = show; // 深リンク用

    // ボタン
    var bPresent = document.getElementById("btn-present");
    var bPrint = document.getElementById("btn-print");
    if (bPresent) bPresent.addEventListener("click", enterPresent);
    if (bPrint) bPrint.addEventListener("click", function () { window.print(); });

    // 発表バー
    var bar = document.createElement("div");
    bar.className = "present-bar";
    bar.innerHTML =
      '<button id="pv">◀ もどる</button>' +
      '<span class="counter" id="counter">1 / ' + slides.length + "</span>" +
      '<button id="nx">つぎ ▶</button>' +
      '<button id="ex">終了 (Esc)</button>';
    document.body.appendChild(bar);
    document.getElementById("pv").addEventListener("click", function () { show(current - 1); });
    document.getElementById("nx").addEventListener("click", function () { show(current + 1); });
    document.getElementById("ex").addEventListener("click", exitPresent);

    // クリックで次へ（発表モード時、ボタン以外）
    document.addEventListener("click", function (e) {
      if (!document.body.classList.contains("mode-present")) return;
      if (e.target.closest(".present-bar")) return;
      show(current + 1);
    });

    // キーボード
    document.addEventListener("keydown", function (e) {
      var present = document.body.classList.contains("mode-present");
      if (e.key === "Escape" && present) { exitPresent(); return; }
      if (!present) return;
      if (e.key === "ArrowRight" || e.key === "PageDown" || e.key === " ") { e.preventDefault(); show(current + 1); }
      else if (e.key === "ArrowLeft" || e.key === "PageUp") { e.preventDefault(); show(current - 1); }
      else if (e.key === "Home") { show(0); }
      else if (e.key === "End") { show(slides.length - 1); }
    });
  }

  if (document.readyState === "loading") {
    document.addEventListener("DOMContentLoaded", init);
  } else { init(); }
})();

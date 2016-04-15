$.fn.styledSelect = function (options) {
    var isFF2 = false;
    var prefs = {
        coverClass: 'select-replace-cover',
        innerClass: 'select-replace',
        adjustPosition: {
            top: 0,
            left: 0
        },
        selectOpacity: 100
    }
    if (options)
        $.extend(prefs, options);
    return this.each(function () {
        if (isFF2)
            return false;
        var selElm = $(this);

        /*if (!selElm.next('span.' + prefs.innerClass).length) {
            selElm.wrap('<span><' + '/span>');
            selElm.after('<span><' + '/span>');
            var selReplace = selElm.next();
            var selCover = selElm.parent();

            selCover.append('<span class="arr"><' + '/span>');
            selElm.css({
                'opacity': prefs.selectOpacity,
                'visibility': 'visible',
                'position': 'absolute',
                'top': 0,
                'left': 0,
                'display': 'inline',
                'z-index': 1
            });
            selCover.addClass(prefs.coverClass).css({
                'display': 'inline-block',
                'position': 'relative',
                'top': prefs.adjustPosition.top,
                'left': prefs.adjustPosition.left,
                'z-index': 0,
                'vertical-align': 'middle',
                'text-align': 'left'
            });
            selReplace.addClass(prefs.innerClass).css({
                'display': 'block',
                'white-space': 'nowrap'
            });

            selElm.bind('change', function () {
                $(this).next().text(this.options[this.selectedIndex].text);
            }).bind('resize', function () {
                $(this).parent().width($(this).width() + 'px');
            });
            selElm.trigger('change').trigger('resize');
        } else {
            var selElm = $(this);
            var selReplace = selElm.next();
            var selCover = selElm.parent();
            selElm.css({
                'opacity': prefs.selectOpacity,
                'visibility': 'visible',
                'position': 'absolute',
                'top': 0,
                'left': 0,
                'display': 'inline',
                'z-index': 1
            });
            selCover.css({
                'display': 'inline-block',
                'position': 'relative',
                'top': prefs.adjustPosition.top,
                'left': prefs.adjustPosition.left,
                'z-index': 0,
                'vertical-align': 'middle',
                'text-align': 'left'
            });
            selReplace.css({
                'display': 'block',
                'white-space': 'nowrap'
            });

        }*/

    });
};
$.fn.placeHolder = function (e) {
    var elem = this;
    elem.each(function () {
        if ($(this).val() === "")
            $(this).val($(this).attr('value-placeholder'));
    });
    elem.on('focus', function () {
        if ($(this).val() === $(this).attr('value-placeholder'))
            $(this).val("");
    });
    elem.on('blur', function () {
        if ($(this).val() === "")
            $(this).val($(this).attr('value-placeholder'));
    });
};
function homeTab() {
    //init
    var hash = window.location.hash.substr(1);

    if (hash === '' || hash === 'undefined') {
        hash = 'pulsa';
    }

    if ($('.home_tab .content_tab #' + hash) !== 'undefined') {
        $('.home_tab .content_tab #' + hash).css('display', 'block');
        $('section.h_middle_top .home_tab .nav_tab a[target=' + hash + ']').addClass('active');
        $('section.h_middle_top .home_tab .nav_tab a[target=' + hash + ']').siblings().removeClass('active');
    }

    $(".home_tab .nav_tab a").click(function (e) {
        e.preventDefault();
        if (!$(this).hasClass('active')) {
            target = $(this).attr('target');

            $(".home_tab .nav_tab a").removeClass('active');
            $(this).addClass('active');

            $(".home_tab .content_tab .tab").slideUp(300);
            $(".home_tab .content_tab .tab#" + target).slideDown(300);
        }
    });

}
function kreditTab() {
    //init
    var target = $(".akun .nav_tab a.active").attr('target');
    $(".akun .content_tab #" + target).css('display', 'block');

    $(".akun .nav_tab a").click(function (e) {
        e.preventDefault();
        if (!$(this).hasClass('active')) {
            target = $(this).attr('target');

            $(".akun .nav_tab a").removeClass('active');
            $(this).addClass('active');

            $(".akun .content_tab .tab").slideUp(300);
            $(".akun .content_tab .tab#" + target).slideDown(300);
        }
    });

}
function dropTransaksi() {
    $(".drop_transaksi").click(function () {
        $(this).children(".box_drop").slideToggle();
    });
}
function dropDownHeader() {
    $("header .right_link .box").hover(function () {
        $(this).children(".box_drop").stop().fadeIn(300);
    }, function () {
        $(this).children(".box_drop").stop().fadeOut(300);
    });
}

function dropDownCart() {
    $("header .right_link .box a.tooglecart").click(function () {
        $("#shopping-cart").stop().fadeToggle(300);
    });
}

function  openPop(selector) {
    $("body").css("overflow", "hidden");
    $(selector).delay(100).fadeIn(300)
}
function closePop(selector) {
    $(selector).delay(100).fadeOut(300, function () {
        $("body").css("overflow", "auto");
    });
}
function popupLogin() {
    $("header .right_link .login").click(function (e) {
        e.preventDefault();
        openPop(".wrap_popup#login");
    });
    $(".wrap_popup .close").click(function (e) {
        e.preventDefault();
        $(this).parents('.wrap_popup').fadeOut(300, function () {
            $("body").css("overflow", "auto");
        });
    });
}
function radioButton() {
    $(".radio").each(function () {
        var radio = $(this).children("input[type=radio]");
        if (radio.is(":checked"))
            $(this).addClass('active')
    });
    $(".radio").click(function () {
        var
                radio = $(this).children("input[type=radio]"),
                group = $("input[name=" + radio.attr('name') + "]");

        group.each(function () {
            $(this).parent('.radio').removeClass('active');
        });

        if (!$(this).hasClass('active'))
            $(this).addClass('active');
    });

    $(".payment_option .radio").click(function () {
        var target = $(this).attr('target_desc');
        if (target !== undefined) {
            $(".desc_payment_method .desc").stop().slideUp(300);

            if ($(".desc_payment_method .desc." + target).length)
                $(".desc_payment_method .desc." + target).stop().slideDown(300);
        }
    });
}

function accordion() {
    $(".row.accordion .label").css('cursor','pointer');
    $(".accordion").each(function () {
        if ($(this).hasClass('active')) {
            $(this).children(".content").slideDown(300);
        }
        if ($(this).hasClass('deactive')) {
            $(this).children(".content").hide();
        }
    });

    $(".row.accordion .label").click(function () {
        if ($(this).parent('.accordion ').hasClass('active')) {
            $(this).parent('.accordion ').children(".content").slideUp(300);
            $(this).parent('.accordion ').removeClass('active');
            $(this).children('.ico').html('+');
        } else {
            $(this).parent('.accordion ').children(".content").slideDown(300);
            $(this).parent('.accordion ').addClass('active');
             $(this).children('.ico').html('-');
        }
    });
}
function  voucherPop() {
    var act = {};

    act.setDataDetail = function (e, popup) {
        var
                data_popup = e.find(".data_popup"),
                content_pop = popup.find(".content_pop");

        if (!content_pop.children(".image").find('img').length) {
            content_pop.children(".image").append('<img src="' + data_popup.find(".img").html() + '" />');
        } else {
            content_pop.children(".image").find("img").attr('src', data_popup.find(".img").html());
        }
        content_pop.children("h3").html(data_popup.find("h3").html());
        content_pop.children("h4").html(data_popup.find("h4").html());
        content_pop.children("h5").html(data_popup.find("h5").html());
        content_pop.children("p").html(data_popup.find(".tnc_desc").html());
        content_pop.find("input[name=voucher_id]").val(data_popup.attr('id'));

    };
    $(".box_voucher .image,.box_voucher a.detail").click(function (e) {
        e.preventDefault();
        var e_box = $(this).parents(".box_voucher");
        $("#detail_voucher").attr('data-index', e_box.index());

        if (e_box.parent().find(".box_voucher").length < 2) {
            $("#detail_voucher").find(".nav_pop").css('display', 'none');
        } else {
            $("#detail_voucher").find(".nav_pop").css('display', 'block');
        }
        if (!e_box.hasClass('offline')) {
            openPop("#detail_voucher");
            $("#detail_voucher").find('.redeem').css('display', 'none');
            act.setDataDetail(e_box, $("#detail_voucher"));
        }
    });

    $(".wrap_popup.voucher .nav_pop a").click(function (e) {
        e.preventDefault();

        var index = parseInt($("#detail_voucher").attr('data-index'));
        var flag = false;
        if ($(this).hasClass('next')) {
            index = index + 1;
            if ($(".box_voucher").eq(index).length)
                flag = true
        } else {
            index = index - 1;
            if ($(".box_voucher").eq(index).length)
                flag = true

        }
        if (flag) {
            $("#detail_voucher .box_popup").fadeOut(200, function () {
                act.setDataDetail($(".box_voucher").eq(index), $("#detail_voucher"));
            });
            $("#detail_voucher .box_popup").delay(300).fadeIn(200);

            $("#detail_voucher").attr('data-index', index);
        } else {
            closePop("#detail_voucher");
        }
    });

    $(document).keyup(function(e) {
         if (e.keyCode == 27) { // escape key maps to keycode `27`
            closePop("#detail_voucher");
        }
    });

}


function hoverShare() {
    var elem = $('.share');
    elem.hover(function () {
        $(this).find('.show_share').stop().fadeIn();
    }, function () {
        $(this).find('.show_share').stop().fadeOut();
    });
}

function menuMobile() {
    $(".nav_mobile .toggle").click(function (e) {
        e.preventDefault();
        $(this).parent().children(".box_drop").slideToggle(300);
    });


    $(".mobile-download .close").click(function (e) {
        e.preventDefault();
        $(".mobile-download").css('display', 'none');
    });
}

function randomTheme() {
    /*if ($("body").hasClass('home')) {
        var class_rand = ['', 'blue', 'green', 'red'];
        var random = Math.floor((Math.random() * 4) + 0);

        $("body").addClass(class_rand[random]);
    }*/
}

function stepIsiPulsa() {
    $(".toogle_step").click(function () {
        $(this).parents('.tab').find(".next_step").slideDown(300);
    });
}

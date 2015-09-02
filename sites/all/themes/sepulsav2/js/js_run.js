$(document).ready(function () {
    $("input[type=text]").placeHolder();//placeholder custom
    $(".form input[type=password]").placeHolder();//placeholder custom
    $(".voucher_vendor .slider").bxSlider({
        slideWidth: "175px",
        minSlides: 2,
        maxSlides: 6,
        ticker: true,
        speed: 20000
    });// carousel voucher vendor

    $(".slider_voucher .slider").bxSlider({
        slideWidth: "315px",
        minSlides:1,
        maxSlides: 3,
        marginSlide: 7
    });// carousel voucher 

    $(".promo_banner .slider").bxSlider({
        auto: true,
        mode: 'fade'
    });// carousel voucher vendor

    randomTheme();// random theme at home
    homeTab(); //tab switch at home
    kreditTab(); //tab switch at home
    dropTransaksi(); //drop transaksi combobox
    dropDownHeader(); //dropdown menu 
    popupLogin(); //open popup login    
    stepIsiPulsa();//toogle show nextstep(isi pulsa)
    radioButton(); //radio button custom
    accordion(); //  accordion other content (faq & etc)
    voucherPop();//voucher popup
    menuMobile()//menu mobile

    $(".form.style_1 select").styledSelect({
        coverClass: 'cover_combo',
        innerClass: 'inner_combo'
    });
    $(".form_cc select").styledSelect({
        coverClass: 'cover_combo',
        innerClass: 'inner_combo'
    });
    $(".form select").styledSelect({
        coverClass: 'cover_combo',
        innerClass: 'inner_combo'
    });
    $(".filter select").styledSelect({
        coverClass: 'cover_combo',
        innerClass: 'inner_combo'
    });

    $(".back_top").click(function (e) {
        e.preventDefault();
        $("body,html").animate({
            scrollTop: 0
        }, 1000);
    });



    /* blog
     ================================================================ */
    $(".search_select select").styledSelect({
        coverClass: 'cover_combo',
        innerClass: 'inner_combo'
    });
    hoverShare(); // hover share blog



});

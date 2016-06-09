$(document).ready(function () {
    $("input[type=text]").placeHolder();//placeholder custom
    $(".form input[type=password]").placeHolder();//placeholder custom
    $(".voucher_vendor .slider").bxSlider({
        slideWidth: "265px",
        minSlides: 1,
        maxSlides: 4
        /*ticker: true,
        tickerHover: true,
        speed: 40000*/
    });// carousel voucher vendor

    $(".slider_voucher .slider").bxSlider({
        slideWidth: "315px",
        minSlides:1,
        maxSlides: 3,
        marginSlide: 7
    });// carousel voucher

    $(".promo_banner .slider > .slide").length

    $(".promo_banner .slider").bxSlider({
        auto: ($(".promo_banner .slider > .slide").length > 1) ? true: false,
        mode: 'fade',
        pager: false,
        pause: 7000,
        default: 4000,
        autoHover: true,
        tickerHover: true,
        controls : ($(".promo_banner .slider > .slide").length > 1) ? true: false
    });// carousel voucher vendor


    $(".banner .slider").bxSlider({
        auto: true,
        mode: 'fade',
        controls: false,
        speed: 1000,
        pause: 7000
    });// carousel main banner

    randomTheme();// random theme at home
    homeTab(); //tab switch at home
    kreditTab(); //tab switch at home
    dropTransaksi(); //drop transaksi combobox
    dropDownHeader(); //dropdown menu
    dropDownCart(); //dropdown menu
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

    $('#edit-add-charge, input[id^=edit-charge]').click(function() {
      return confirm('Apakah Anda yakin ingin melanjutkan pembayaran tanpa memilih Voucher?');
    });

    /* blog
     ================================================================ */
    $(".search_select select").styledSelect({
        coverClass: 'cover_combo',
        innerClass: 'inner_combo'
    });
    hoverShare(); // hover share blog

    $(".banner").click(function (event) {
      if(event.target != $('div.a')[0])
        $("#shopping-cart").fadeOut();
    });

});

Page({
  data: {
    device_name: 32 ,
  },
  
  onLoad: function (options) {
    var _this = this;
    //console.log(options.device_id);// 打印设备ID
    //console.log(app.data.api_key);// 打印API_KEY
    wx.request({
      url: 'https://api.heclouds.com/devices/562234373/datapoints',
      header: {
        'api-key': 'Rh3VomUuIy1yXEpJURmUlx7npRE='
      },
      onLoad: function (res) {
        console.log(res.data);
        _this.setData(
          {
            device_name: res.data.data.datastreams[0].datapoints[0].value,

          });
      },
      fail: function () {
        wx.showToast({
          title: '与服务器通信失败',
          icon: 'fail',
          duration: 2000
        })
      }
    })
  },
  lookfor:function(){
    wx.navigateTo
   ({
      url: '../case/case',   //注意switchTab只能跳转到带有tab的页面，不能跳转到不带tab的页面
    })
  },
 
  switchpage: function () {
    wx.navigateTo({
      url: '../index/index',
    })
  },
  switchpage1: function () {
    wx.navigateTo({
      url: '../clean/clean',
    })
  },

})